/*
 * Copyright (C) 2016--2019 Richard Preen <rpreen@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **************
 * Description: 
 **************
 * The MLP neural network classifier computed prediction module.
 *
 * Creates a weight vector representing an MLP neural network to calculate the
 * expected value given a problem instance and adapts the weights using the
 * backpropagation algorithm.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "xcsf.h"
#include "utils.h"
#include "cl.h"
#include "neural_activations.h"
#include "neural.h"
#include "prediction.h"
#include "pred_neural.h"

typedef struct PRED_NEURAL {
    BPN bpn;
} PRED_NEURAL;

void pred_neural_init(XCSF *xcsf, CL *c)
{
    PRED_NEURAL *new = malloc(sizeof(PRED_NEURAL));
    int neurons[3] = {xcsf->num_x_vars, xcsf->NUM_HIDDEN_NEURONS, xcsf->num_y_vars};
    int activations[2] = {xcsf->HIDDEN_NEURON_ACTIVATION, IDENTITY};
    neural_init(xcsf, &new->bpn, 3, neurons, activations);
    c->pred = new;
}

void pred_neural_free(XCSF *xcsf, CL *c)
{
    PRED_NEURAL *pred = c->pred;
    neural_free(xcsf, &pred->bpn);
    free(pred);
}

void pred_neural_copy(XCSF *xcsf, CL *to, CL *from)
{
    PRED_NEURAL *new = malloc(sizeof(PRED_NEURAL));
    PRED_NEURAL *from_pred = from->pred;
    neural_copy(xcsf, &new->bpn, &from_pred->bpn);
    to->pred = new;
}

void pred_neural_update(XCSF *xcsf, CL *c, double *x, double *y)
{
    PRED_NEURAL *pred = c->pred;
    neural_learn(xcsf, &pred->bpn, y, x);
}

double *pred_neural_compute(XCSF *xcsf, CL *c, double *x)
{
    PRED_NEURAL *pred = c->pred;
    neural_propagate(xcsf, &pred->bpn, x);
    for(int i = 0; i < xcsf->num_y_vars; i++) {
        c->prediction[i] = neural_output(xcsf, &pred->bpn, i);
    }
    return c->prediction;
}

void pred_neural_print(XCSF *xcsf, CL *c)
{
    PRED_NEURAL *pred = c->pred;
    neural_print(xcsf, &pred->bpn, true);
}  
 
_Bool pred_neural_crossover(XCSF *xcsf, CL *c1, CL *c2)
{
    (void)xcsf; (void)c1; (void)c2;
    return false;
}

_Bool pred_neural_mutate(XCSF *xcsf, CL *c)
{
    PRED_NEURAL *pred = c->pred;
    return neural_mutate(xcsf, &pred->bpn);
}
