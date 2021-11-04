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
 * @file neural_layer_dropout.h
 * @author Richard Preen <rpreen@gmail.com>
 * @copyright The Authors.
 * @date 2016--2021.
 * @brief An implementation of a dropout layer.
 */

#pragma once

#include "neural_layer.h"

void
neural_layer_dropout_init(struct Layer *l, const struct ArgsLayer *args);

struct Layer *
neural_layer_dropout_copy(const struct Layer *src);

void
neural_layer_dropout_rand(struct Layer *l);

void
neural_layer_dropout_forward(const struct Layer *l, const struct Net *net,
                             const double *input);

void
neural_layer_dropout_backward(const struct Layer *l, const struct Net *net,
                              const double *input, double *delta);

void
neural_layer_dropout_update(const struct Layer *l);

void
neural_layer_dropout_print(const struct Layer *l, const bool print_weights);

bool
neural_layer_dropout_mutate(struct Layer *l);

void
neural_layer_dropout_free(const struct Layer *l);

double *
neural_layer_dropout_output(const struct Layer *l);

size_t
neural_layer_dropout_save(const struct Layer *l, FILE *fp);

size_t
neural_layer_dropout_load(struct Layer *l, FILE *fp);

void
neural_layer_dropout_resize(struct Layer *l, const struct Layer *prev);

const char *
neural_layer_dropout_json(const struct Layer *l, const bool return_weights);

/**
 * @brief Neural dropout layer implemented functions.
 */
static struct LayerVtbl const layer_dropout_vtbl = {
    &neural_layer_dropout_init,     &neural_layer_dropout_mutate,
    &neural_layer_dropout_resize,   &neural_layer_dropout_copy,
    &neural_layer_dropout_free,     &neural_layer_dropout_rand,
    &neural_layer_dropout_print,    &neural_layer_dropout_update,
    &neural_layer_dropout_backward, &neural_layer_dropout_forward,
    &neural_layer_dropout_output,   &neural_layer_dropout_save,
    &neural_layer_dropout_load,     &neural_layer_dropout_json
};
