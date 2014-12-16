/*
 * Copyright (C) 2015 Richard Preen <rpreen@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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
typedef struct CL
{
	double *con;
	int con_length;
	double *weights;
	int weights_length;
	double err;
	double fit;
	int num;
	int exp;
	double size;
	int time;
#ifdef SELF_ADAPT_MUTATION
	double *mu;
	int *iset;
	double *gset;
#endif
} CL;

// classifier
void init_cl(CL *c, int size, int time);
void copy_cl(CL *to, CL *from);
void free_cl(CL *c);
void print_cl(CL *c);
double del_vote(CL *c, double avg_fit);
double acc(CL *c);
void update_fit(CL *c, double acc_sum, double acc);
double update_size(CL *c, double num_sum);
_Bool subsumer(CL *c);

// classifier predictions
void update_pre(CL *c, double p, double *state);
double compute_pre(CL *c, double *state);
double update_err(CL *c, double p, double *state);

// classifier conditions
_Bool two_pt_cross(CL *c1, CL *c2);
void con_init(CL *c);
void con_free(CL *c);
void con_copy(CL *to, CL *from);
void con_rand(CL *c);
void con_match(CL *c, double *state);
void con_print(CL *c);
_Bool match(CL *c, double *state);
_Bool mutate(CL *c);
_Bool subsumes(CL *c1, CL *c2);
_Bool general(CL *c1, CL *c2);

// self-adaptive mutation
void sam_init(CL *c);
void sam_free(CL *c);
void sam_copy(CL *to, CL *from);
void sam_adapt(CL *c);       
