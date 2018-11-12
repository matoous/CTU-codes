#ifndef PDV08_MULTIPLY_H
#define PDV08_MULTIPLY_H

#include "sparse.h"

sparse_vector multiply_sequential(const sparse_matrix & A, const sparse_vector & x);
sparse_vector multiply_parallel(const sparse_matrix & A, const sparse_vector & x);

#endif //PDV08_MULTIPLY_H
