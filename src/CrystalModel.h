#ifndef CRYSTAL_MODEL_H
#define CRYSTAL_MODEL_H

#include <stdbool.h>

typedef struct crystal_model_t CrystalModel;

CrystalModel *
CrystalModel_create(int bath_width, int r_start, int r_escape);
void
CrystalModel_destroy(CrystalModel *self);
bool
CrystalModel_crystallize_one_ion(CrystalModel *self);
bool
CrystalModel_get_model_value(CrystalModel const *self,
			     int x,
			     int y);
void
CrystalModel_reset(CrystalModel *self);
int
CrystalModel_x_bath_to_model_rep(CrystalModel const *self,
				 int x);
int
CrystalModel_y_bath_to_model_rep(CrystalModel const *self,
				 int y);
int
CrystalModel_get_x(CrystalModel const *self);
int
CrystalModel_get_y(CrystalModel const *self);
int
CrystalModel_get_r_bounds(CrystalModel const *self);
int
CrystalModel_get_radius(CrystalModel const *self);
int
CrystalModel_get_bath_width(CrystalModel const *self);
bool
CrystalModel_run_some_steps(CrystalModel *self,
			    int steps);
void
CrystalModel_srand(CrystalModel *self,
		   int seed);
char const *
CrystalModel_to_string(CrystalModel const *self);

#endif //CRYSTAL_MODEL_H
