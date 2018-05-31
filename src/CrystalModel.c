#include "CrystalModel.h"

#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <math.h>

#include "Matrix.h"
#include "Point.h"
#include "random.h"

typedef struct string_t
{
  char *str;
  int len;
} String;

String *
String_alloc(int size)
{
  String *self = calloc(1, sizeof(String));
  self->str = calloc(size+1, sizeof(char));
  self->len = size;
  return self;
}

void
String_dealloc(String *self)
{
  if (!self) { return; }

  free(self->str);
  free(self);
}

struct crystal_model_t
{
  Matrix *m_mat;
  Point m_p;
  int m_r_start;
  int m_r_escape;
  int m_bath_width;
  String *m_s;
};

static bool
outside_circle(int rEscape,
	       Point const *p);
static bool
any_neighbours(CrystalModel const *self,
	       Point const *p);
static void
drop_new_ion(int r_start,
	     Point *p);
static void
step_once(Point *p);
static bool *
bath_at(CrystalModel const *self,
	int x,
	int y);

static int const dx[] = { 1, -1, 0,  0};
static int const dy[] = { 0,  0, 1, -1};

CrystalModel *
CrystalModel_create(int bath_width, int r_start, int r_escape)
{
  CrystalModel *self = (CrystalModel *)calloc(1, sizeof(CrystalModel));
  
  self->m_r_start = r_start;
  self->m_r_escape = r_escape;
  self->m_bath_width = bath_width;
  self->m_mat = Matrix_create(bath_width);
  self->m_s = String_alloc((bath_width+2)*(bath_width+2) + 1);

  CrystalModel_reset(self);
  return self;
}

void
CrystalModel_destroy(CrystalModel *self)
{
  if (!self) { return; }

  String_dealloc(self->m_s);
  Matrix_destroy(self->m_mat);
  free(self);
}

bool
CrystalModel_crystallize_one_ion(CrystalModel *self) {
  Point p = { 0, 0 };
  for (drop_new_ion(self->m_r_start, &p); !any_neighbours(self, &p); step_once(&p)) {
    if (outside_circle(self->m_r_escape, &p)) {
      return CrystalModel_crystallize_one_ion(self);
    }
  }
  self->m_p = p;
  *bath_at(self, p.x, p.y) = true;
  return !outside_circle(self->m_r_start, &self->m_p);
}

bool
CrystalModel_get_model_value(CrystalModel const *self,
			     int x,
			     int y)
{
  return *bath_at(self, x, y);
}

void
CrystalModel_reset(CrystalModel *self)
{
  Matrix_clear(self->m_mat);
  *bath_at(self, 0, 0) = true;
}

int
CrystalModel_x_bath_to_model_rep(CrystalModel const *self,
				 int x)
{
  return x + self->m_bath_width/2;
}

int
CrystalModel_y_bath_to_model_rep(CrystalModel const *self,
				 int y)
{
  return self->m_bath_width/2 - y;
}

int
CrystalModel_get_x(CrystalModel const *self)
{
  return self->m_p.x;
}

int
CrystalModel_get_y(CrystalModel const *self)
{
  return self->m_p.y;
}

int
CrystalModel_get_r_bounds(CrystalModel const *self)
{
  return self->m_r_start;
}

int
CrystalModel_get_radius(CrystalModel const *self)
{
  return self->m_r_escape;
}

int
CrystalModel_get_bath_width(CrystalModel const *self)
{
  return self->m_bath_width;
}

bool
CrystalModel_run_some_steps(CrystalModel *self,
			    int steps)
{
  if (steps > 0) {
    return CrystalModel_crystallize_one_ion(self) && CrystalModel_run_some_steps(self, steps-1);
  }
  return true;
}

void
CrystalModel_srand(CrystalModel *self,
		   int seed)
{
  (void)self;
  cs_srand(seed);
}

char const *
CrystalModel_to_string(CrystalModel const *self)
{
  int x = CrystalModel_get_x(self);
  int y = CrystalModel_get_y(self);
  int size = CrystalModel_get_radius(self);
  bzero(self->m_s->str, self->m_s->len);
  char *s;
  char const *out = s = self->m_s->str;
  for(int i = -(size+1); i < size+1; i++) {
    *s++ = '-';
  }
  *s++ = '\n';
  for(int i = -size; i < size; i++) {
    *s++ = '|';
    for(int j = -size; j < size; j++) {
      if (CrystalModel_get_model_value(self, i, j)) {
	*s++ = (i == x && j == y) ? '#' : '*';
      } else {
	*s++ = ' ';
      }
    }
    *s++ = '|';
    *s++ = '\n';
  }
  for(int i = -(size+1); i < size+1; i++) {
    *s++ = '-';
  }
  *s++ = '\n';
  return out;
}

static bool
outside_circle(int rEscape,
	       Point const *p)
{
  return (int)sqrt(p->x*p->x + p->y*p->y) >= rEscape;
}

static bool
any_neighbours(CrystalModel const *self,
	       Point const *p)
{
  return CrystalModel_get_model_value(self, p->x+1, p->y)
    || CrystalModel_get_model_value(self, p->x-1, p->y)
    || CrystalModel_get_model_value(self, p->x, p->y+1)
    || CrystalModel_get_model_value(self, p->x, p->y-1);
}

static void
drop_new_ion(int r_start,
	     Point *p)
{
  double alpha = 2 * M_PI * cs_drand();
  p->x = (int)(r_start*cos(alpha));
  p->y = (int)(r_start*sin(alpha));
}

static void
step_once(Point *p)
{
  int i = cs_rand() & 3;
  p->x += dx[i];
  p->y += dy[i];
}

static bool *
bath_at(CrystalModel const *self,
	int x,
	int y)
{
  return Matrix_at(self->m_mat,
		   CrystalModel_x_bath_to_model_rep(self, x),
		   CrystalModel_y_bath_to_model_rep(self, y));
}
