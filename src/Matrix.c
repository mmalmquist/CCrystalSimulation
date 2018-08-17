#include "Matrix.h"

#include <stdlib.h>
#include <string.h>

extern Matrix *
Matrix_create(unsigned size)
{
  Matrix *self = (Matrix *)calloc(1, sizeof(Matrix));
  
  self->_array = (matrix_t *)calloc(size*size, sizeof(matrix_t));
  self->_size = size;
  
  return self;
}

extern void
Matrix_destroy(Matrix *self)
{
  if (!self) { return; }

  free(self->_array); self->_array = NULL;
  free(self);
}

extern void
Matrix_clear(Matrix *self)
{
  memset(self->_array, 0, self->_size*self->_size);
}
