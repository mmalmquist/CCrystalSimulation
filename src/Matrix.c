#include "Matrix.h"

#include <stdlib.h>
#include <string.h>

Matrix *
Matrix_create(int size)
{
  Matrix *self = (Matrix *)calloc(1, sizeof(Matrix));
  
  self->m_array = (bool *)calloc(size*size, sizeof(bool));
  self->m_size = size;
  
  return self;
}

void
Matrix_destroy(Matrix *self)
{
  if (!self) { return; }

  free(self->m_array);
  free(self);
}

void
Matrix_clear(Matrix *self)
{
  bzero(self->m_array, self->m_size*self->m_size);
}
