#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdbool.h>

typedef struct matrix_t
{
  bool *m_array;
  int m_size;
} Matrix;

Matrix *
Matrix_create(int size);

void
Matrix_destroy(Matrix *self);

void
Matrix_clear(Matrix *self);

static inline bool *
Matrix_at(Matrix *self, int x, int y)
{
  return self->m_array + (x + y*self->m_size);
}

static inline bool const *
Matrix_at_const(Matrix const *self, int x, int y)
{
  return self->m_array + (x + y*self->m_size);
}

#endif // MATRIX_H_
