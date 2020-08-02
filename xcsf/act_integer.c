/*
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
 */

/**
 * @file act_integer.c
 * @author Richard Preen <rpreen@gmail.com>
 * @copyright The Authors.
 * @date 2015--2020.
 * @brief integer action functions.
 */

#include "act_integer.h"
#include "sam.h"
#include "utils.h"

#define N_MU (1) //!< Number of integer action mutation rates

_Bool
act_integer_crossover(const struct XCSF *xcsf, const struct CL *c1,
                      const struct CL *c2)
{
    (void) xcsf;
    (void) c1;
    (void) c2;
    return false;
}

_Bool
act_integer_general(const struct XCSF *xcsf, const struct CL *c1,
                    const struct CL *c2)
{
    (void) xcsf;
    const ACT_INTEGER *act1 = c1->act;
    const ACT_INTEGER *act2 = c2->act;
    if (act1->action != act2->action) {
        return false;
    }
    return true;
}

_Bool
act_integer_mutate(const struct XCSF *xcsf, const struct CL *c)
{
    ACT_INTEGER *act = c->act;
    sam_adapt(xcsf, act->mu, N_MU);
    if (rand_uniform(0, 1) < act->mu[0]) {
        int old = act->action;
        act->action = irand_uniform(0, xcsf->n_actions);
        if (old != act->action) {
            return true;
        }
    }
    return false;
}

int
act_integer_compute(const struct XCSF *xcsf, const struct CL *c,
                    const double *x)
{
    (void) xcsf;
    (void) x;
    const ACT_INTEGER *act = c->act;
    return act->action;
}

void
act_integer_copy(const struct XCSF *xcsf, struct CL *dest, const struct CL *src)
{
    (void) xcsf;
    ACT_INTEGER *new = malloc(sizeof(ACT_INTEGER));
    const ACT_INTEGER *src_act = src->act;
    new->action = src_act->action;
    new->mu = malloc(sizeof(double) * N_MU);
    memcpy(new->mu, src_act->mu, sizeof(double) * N_MU);
    dest->act = new;
}

void
act_integer_print(const struct XCSF *xcsf, const struct CL *c)
{
    (void) xcsf;
    const ACT_INTEGER *act = c->act;
    printf("%d\n", act->action);
}

void
act_integer_cover(const struct XCSF *xcsf, const struct CL *c, const double *x,
                  int action)
{
    (void) xcsf;
    (void) x;
    ACT_INTEGER *act = c->act;
    act->action = action;
}

void
act_integer_free(const struct XCSF *xcsf, const struct CL *c)
{
    (void) xcsf;
    const ACT_INTEGER *act = c->act;
    free(act->mu);
    free(c->act);
}

void
act_integer_init(const struct XCSF *xcsf, struct CL *c)
{
    ACT_INTEGER *new = malloc(sizeof(ACT_INTEGER));
    new->mu = malloc(sizeof(double) * N_MU);
    sam_init(xcsf, new->mu, N_MU);
    new->action = irand_uniform(0, xcsf->n_actions);
    c->act = new;
}

void
act_integer_update(const struct XCSF *xcsf, const struct CL *c, const double *x,
                   const double *y)
{
    (void) xcsf;
    (void) c;
    (void) x;
    (void) y;
}

size_t
act_integer_save(const struct XCSF *xcsf, const struct CL *c, FILE *fp)
{
    (void) xcsf;
    size_t s = 0;
    const ACT_INTEGER *act = c->act;
    s += fwrite(&act->action, sizeof(int), 1, fp);
    s += fwrite(act->mu, sizeof(double), N_MU, fp);
    return s;
}

size_t
act_integer_load(const struct XCSF *xcsf, struct CL *c, FILE *fp)
{
    (void) xcsf;
    size_t s = 0;
    ACT_INTEGER *new = malloc(sizeof(ACT_INTEGER));
    s += fread(&new->action, sizeof(int), 1, fp);
    new->mu = malloc(sizeof(double) * N_MU);
    s += fread(new->mu, sizeof(double), N_MU, fp);
    c->act = new;
    return s;
}
