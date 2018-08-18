#include "CrystalModel.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Matrix.h"
#include "Point.h"
#include "random.h"

struct crystal_model_t
{
  Matrix *_mat;
  Point _p;
  unsigned _r_start;
  unsigned _r_escape;
  char *_s;
};

static int
outside_circle(unsigned rEscape,
	       Point const *p);
static int
any_neighbours(CrystalModel const * self,
	       Point const * p);
static void
drop_new_ion(unsigned r_start,
	     Point *p);
static void
step_once(Point *p);
static matrix_t *
bath_at(CrystalModel const *self,
	int x,
	int y);

static Point const dp[] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

extern CrystalModel *
CrystalModel_create(Matrix *mat,
		    unsigned r_start,
		    unsigned r_escape)
{
  CrystalModel *self = (CrystalModel *)calloc(1, sizeof(CrystalModel));
  
  self->_r_start = r_start;
  self->_r_escape = r_escape;
  self->_mat = mat;
  self->_s = (char *) calloc((Matrix_size(mat)+2)*(Matrix_size(mat)+2) + 1, sizeof(char));

  CrystalModel_reset(self);
  return self;
}

extern void
CrystalModel_destroy(CrystalModel *self)
{
  if (!self) { return; }

  free(self->_s); self->_s = NULL;
  free(self);
}

extern int
CrystalModel_crystallize_one_ion(CrystalModel *self) {
  Point p = { 0, 0 };
  for (drop_new_ion(self->_r_start, &p); !any_neighbours(self, &p); step_once(&p)) {
    if (outside_circle(self->_r_escape, &p)) {
      drop_new_ion(self->_r_start, &p);
    }
  }
  self->_p = p;
  *bath_at(self, p.x, p.y) = 1;
  return !outside_circle(self->_r_start, &self->_p);
}

extern int
CrystalModel_get_model_value(CrystalModel const *self,
			     int x,
			     int y)
{
  return *bath_at(self, x, y);
}

extern void
CrystalModel_reset(CrystalModel *self)
{
  Matrix_clear(self->_mat);
  *bath_at(self, 0, 0) = 1;
}

extern unsigned
CrystalModel_x_bath_to_model_rep(CrystalModel const *self,
				 int x)
{
  return x + Matrix_size(self->_mat)/2;
}

extern unsigned
CrystalModel_y_bath_to_model_rep(CrystalModel const *self,
				 int y)
{
  return Matrix_size(self->_mat)/2 - y;
}

extern int
CrystalModel_get_x(CrystalModel const *self)
{
  return self->_p.x;
}

extern int
CrystalModel_get_y(CrystalModel const *self)
{
  return self->_p.y;
}

extern unsigned
CrystalModel_get_r_bounds(CrystalModel const *self)
{
  return self->_r_start;
}

extern unsigned
CrystalModel_get_radius(CrystalModel const *self)
{
  return self->_r_escape;
}

extern unsigned
CrystalModel_get_bath_width(CrystalModel const *self)
{
  return Matrix_size(self->_mat);
}

extern int
CrystalModel_run_some_steps(CrystalModel *self,
			    unsigned steps)
{
  while (steps-- > 0) {
    if (!CrystalModel_crystallize_one_ion(self)) {
      return 0;
    }
  }
  return 1;
}

extern void
CrystalModel_srand(CrystalModel *self,
		   unsigned seed)
{
  (void)self;
  cs_srand(seed);
}

extern char const *
CrystalModel_to_string(CrystalModel const *self)
{
  int i, j;
  int const x = CrystalModel_get_x(self), y = CrystalModel_get_y(self);
  long const size = CrystalModel_get_radius(self);
  char *s = self->_s;
  
  for(i = -(size+1); i < size+1; i++) {
    *s++ = '-';
  }
  *s++ = '\n';
  for(i = -size; i < size; i++) {
    *s++ = '|';
    for(j = -size; j < size; j++) {
      if (CrystalModel_get_model_value(self, i, j)) {
	*s++ = (i == x && j == y) ? '#' : '*';
      } else {
	*s++ = ' ';
      }
    }
    *s++ = '|';
    *s++ = '\n';
  }
  for(i = -(size+1); i < size+1; i++) {
    *s++ = '-';
  }
  *s++ = '\n';
  return self->_s;
}

static int
outside_circle(unsigned rEscape,
	       Point const *p)
{
  return (unsigned)sqrt(p->x*p->x + p->y*p->y) >= rEscape;
}

static int
any_neighbours(CrystalModel const *self,
	       Point const *p)
{
  return (CrystalModel_get_model_value(self, p->x+dp[0].x, p->y+dp[0].y) ||
	  CrystalModel_get_model_value(self, p->x+dp[1].x, p->y+dp[1].y) ||
	  CrystalModel_get_model_value(self, p->x+dp[2].x, p->y+dp[2].y) ||
	  CrystalModel_get_model_value(self, p->x+dp[3].x, p->y+dp[3].y));
}

static void
drop_new_ion(unsigned r_start,
	     Point *p)
{
  double alpha = 2 * M_PI * cs_drand();
  p->x = (int)(r_start*cos(alpha));
  p->y = (int)(r_start*sin(alpha));
}

static void
step_once(Point *p)
{
  Point const *d = &dp[cs_rand() & 3];
  p->x += d->x;
  p->y += d->y;
}

static matrix_t *
bath_at(CrystalModel const *self,
	int x,
	int y)
{
  return Matrix_at(self->_mat,
		   CrystalModel_x_bath_to_model_rep(self, x),
		   CrystalModel_y_bath_to_model_rep(self, y));
}
