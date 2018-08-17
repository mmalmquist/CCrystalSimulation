#ifndef MATRIX_H_
#define MATRIX_H_

typedef unsigned char matrix_t;

typedef struct {
  matrix_t *_array;
  unsigned _size;
} Matrix;

extern Matrix *
Matrix_create(unsigned size);

extern void
Matrix_destroy(Matrix *self);

extern void
Matrix_clear(Matrix *self);

static inline unsigned
Matrix_size(Matrix *self)
{
  return self->_size;
}

static inline matrix_t *
Matrix_at(Matrix *self,
	  unsigned x,
	  unsigned y)
{
  return self->_array + (x + y*self->_size);
}

static inline matrix_t const *
Matrix_at_const(Matrix const *self,
		unsigned x,
		unsigned y)
{
  return self->_array + (x + y*self->_size);
}

#endif /* MATRIX_H_ */
