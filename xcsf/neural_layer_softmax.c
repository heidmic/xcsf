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
 * @file neural_layer_softmax.c
 * @author Richard Preen <rpreen@gmail.com>
 * @copyright The Authors.
 * @date 2016--2020.
 * @brief An implementation of a softmax layer.
 */

#include "neural_layer_softmax.h"
#include "neural_activations.h"
#include "utils.h"

/**
 * @brief Allocate memory used by a softmax layer.
 * @param [in] l The layer to be allocated memory.
 */
static void
malloc_layer_arrays(struct Layer *l)
{
    if (l->n_inputs < 1 || l->n_inputs > N_INPUTS_MAX) {
        printf("neural_layer_softmax: malloc() invalid size\n");
        l->n_inputs = 1;
        exit(EXIT_FAILURE);
    }
    l->output = calloc(l->n_inputs, sizeof(double));
    l->delta = calloc(l->n_inputs, sizeof(double));
}

/**
 * @brief Free memory used by a softmax layer.
 * @param [in] l The layer to be freed.
 */
static void
free_layer_arrays(const struct Layer *l)
{
    free(l->output);
    free(l->delta);
}

/**
 * @brief Creates and initialises a softmax layer.
 * @param [in] xcsf The XCSF data structure.
 * @param [in] n_inputs The number of inputs.
 * @param [in] temperature The scaling of the logits.
 * @return A pointer to the new layer.
 */
struct Layer *
neural_layer_softmax_init(const struct XCSF *xcsf, const int n_inputs,
                          const double temperature)
{
    (void) xcsf;
    struct Layer *l = malloc(sizeof(struct Layer));
    layer_init(l);
    l->layer_type = SOFTMAX;
    l->layer_vptr = &layer_softmax_vtbl;
    l->scale = temperature;
    l->n_inputs = n_inputs;
    l->n_outputs = n_inputs;
    l->max_outputs = n_inputs;
    malloc_layer_arrays(l);
    return l;
}

/**
 * @brief Initialises and creates a copy of one softmax layer from another.
 * @param [in] xcsf The XCSF data structure.
 * @param [in] src The source layer.
 * @return A pointer to the new layer.
 */
struct Layer *
neural_layer_softmax_copy(const struct XCSF *xcsf, const struct Layer *src)
{
    (void) xcsf;
    if (src->layer_type != SOFTMAX) {
        printf("neural_layer_softmax_copy(): incorrect source layer type\n");
        exit(EXIT_FAILURE);
    }
    struct Layer *l = malloc(sizeof(struct Layer));
    layer_init(l);
    l->layer_type = src->layer_type;
    l->layer_vptr = src->layer_vptr;
    l->scale = src->scale;
    l->n_inputs = src->n_inputs;
    l->n_outputs = src->n_outputs;
    l->max_outputs = src->max_outputs;
    malloc_layer_arrays(l);
    return l;
}

/**
 * @brief Dummy function since softmax layers have no weights.
 * @param [in] xcsf The XCSF data structure.
 * @param [in] l A softmax layer.
 */
void
neural_layer_softmax_rand(const struct XCSF *xcsf, struct Layer *l)
{
    (void) xcsf;
    (void) l;
}

/**
 * @brief Forward propagates a softmax layer.
 * @param [in] xcsf The XCSF data structure.
 * @param [in] l The layer to forward propagate.
 * @param [in] input The input to the layer.
 */
void
neural_layer_softmax_forward(const struct XCSF *xcsf, const struct Layer *l,
                             const double *input)
{
    (void) xcsf;
    double largest = input[0];
    for (int i = 1; i < l->n_inputs; ++i) {
        if (input[i] > largest) {
            largest = input[i];
        }
    }
    double sum = 0;
    for (int i = 0; i < l->n_inputs; ++i) {
        const double e = exp((input[i] / l->scale) - (largest / l->scale));
        sum += e;
        l->output[i] = e;
    }
    for (int i = 0; i < l->n_inputs; ++i) {
        l->output[i] /= sum;
    }
}

/**
 * @brief Backward propagates a softmax layer.
 * @param [in] xcsf The XCSF data structure.
 * @param [in] l The layer to backward propagate.
 * @param [in] input The input to the layer.
 * @param [out] delta The previous layer's error.
 */
void
neural_layer_softmax_backward(const struct XCSF *xcsf, const struct Layer *l,
                              const double *input, double *delta)
{
    (void) xcsf;
    (void) input;
    if (delta) {
        for (int i = 0; i < l->n_inputs; ++i) {
            delta[i] += l->delta[i];
        }
    }
}

/**
 * @brief Dummy function since a softmax layer has no weights.
 * @param [in] xcsf The XCSF data structure.
 * @param [in] l A softmax layer.
 */
void
neural_layer_softmax_update(const struct XCSF *xcsf, const struct Layer *l)
{
    (void) xcsf;
    (void) l;
}

/**
 * @brief Prints a softmax layer.
 * @param [in] xcsf The XCSF data structure.
 * @param [in] l The layer to print.
 * @param [in] print_weights Whether to print the values of weights and biases.
 */
void
neural_layer_softmax_print(const struct XCSF *xcsf, const struct Layer *l,
                           const bool print_weights)
{
    (void) xcsf;
    (void) print_weights;
    printf("softmax in = %d, out = %d, temp = %f\n", l->n_inputs, l->n_outputs,
           l->scale);
}

/**
 * @brief Dummy function since a softmax layer cannot be mutated.
 * @param [in] xcsf The XCSF data structure.
 * @param [in] l A softmax layer.
 * @return False.
 */
bool
neural_layer_softmax_mutate(const struct XCSF *xcsf, struct Layer *l)
{
    (void) xcsf;
    (void) l;
    return false;
}

/**
 * @brief Resizes a softmax layer if the previous layer has changed size.
 * @param [in] xcsf The XCSF data structure.
 * @param [in] l The layer to resize.
 * @param [in] prev The layer previous to the one being resized.
 */
void
neural_layer_softmax_resize(const struct XCSF *xcsf, struct Layer *l,
                            const struct Layer *prev)
{
    (void) xcsf;
    l->n_inputs = prev->n_outputs;
    l->n_outputs = prev->n_outputs;
    l->max_outputs = prev->n_outputs;
    free_layer_arrays(l);
    malloc_layer_arrays(l);
}

/**
 * @brief Free memory used by a softmax layer.
 * @param [in] xcsf The XCSF data structure.
 * @param [in] l The layer to be freed.
 */
void
neural_layer_softmax_free(const struct XCSF *xcsf, const struct Layer *l)
{
    (void) xcsf;
    free_layer_arrays(l);
}

/**
 * @brief Returns the output from a softmax layer.
 * @param [in] xcsf The XCSF data structure.
 * @param [in] l The layer whose output to return.
 * @return The layer output.
 */
double *
neural_layer_softmax_output(const struct XCSF *xcsf, const struct Layer *l)
{
    (void) xcsf;
    return l->output;
}

/**
 * @brief Writes a softmax layer to a file.
 * @param [in] xcsf The XCSF data structure.
 * @param [in] l The layer to save.
 * @param [in] fp Pointer to the file to be written.
 * @return The number of elements written.
 */
size_t
neural_layer_softmax_save(const struct XCSF *xcsf, const struct Layer *l,
                          FILE *fp)
{
    (void) xcsf;
    size_t s = 0;
    s += fwrite(&l->n_inputs, sizeof(int), 1, fp);
    s += fwrite(&l->n_outputs, sizeof(int), 1, fp);
    s += fwrite(&l->max_outputs, sizeof(int), 1, fp);
    s += fwrite(&l->scale, sizeof(double), 1, fp);
    return s;
}

/**
 * @brief Reads a softmax layer from a file.
 * @param [in] xcsf The XCSF data structure.
 * @param [in] l The layer to load.
 * @param [in] fp Pointer to the file to be read.
 * @return The number of elements read.
 */
size_t
neural_layer_softmax_load(const struct XCSF *xcsf, struct Layer *l, FILE *fp)
{
    (void) xcsf;
    size_t s = 0;
    s += fread(&l->n_inputs, sizeof(int), 1, fp);
    s += fread(&l->n_outputs, sizeof(int), 1, fp);
    s += fread(&l->max_outputs, sizeof(int), 1, fp);
    s += fread(&l->scale, sizeof(double), 1, fp);
    malloc_layer_arrays(l);
    return s;
}
