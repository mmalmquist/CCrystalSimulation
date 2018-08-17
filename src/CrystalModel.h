#ifndef CRYSTAL_MODEL_H
#define CRYSTAL_MODEL_H

#include "Matrix.h"

typedef struct crystal_model_t CrystalModel;

extern CrystalModel *
CrystalModel_create(Matrix *mat,
		    unsigned r_start,
		    unsigned r_escape);
extern void
CrystalModel_destroy(CrystalModel *self);
extern int
CrystalModel_crystallize_one_ion(CrystalModel *self);
extern int
CrystalModel_get_model_value(CrystalModel const *self,
			     int x,
			     int y);
extern void
CrystalModel_reset(CrystalModel *self);
extern unsigned
CrystalModel_x_bath_to_model_rep(CrystalModel const *self,
				 int x);
extern unsigned
CrystalModel_y_bath_to_model_rep(CrystalModel const *self,
				 int y);
extern int
CrystalModel_get_x(CrystalModel const *self);
extern int
CrystalModel_get_y(CrystalModel const *self);
extern unsigned
CrystalModel_get_r_bounds(CrystalModel const *self);
extern unsigned
CrystalModel_get_radius(CrystalModel const *self);
extern unsigned
CrystalModel_get_bath_width(CrystalModel const *self);
extern int
CrystalModel_run_some_steps(CrystalModel *self,
			    unsigned steps);
extern void
CrystalModel_srand(CrystalModel *self,
		   unsigned seed);
extern char const *
CrystalModel_to_string(CrystalModel const *self);

#endif /* CRYSTAL_MODEL_H */
