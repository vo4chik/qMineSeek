#ifndef KERNELS_H
#define KERNELS_H

#include "doublearray2d.h"

namespace kernels {
	DoubleArray2D Determinism();
	DoubleArray2D DiagCastle();
	DoubleArray2D VerySparse();
	DoubleArray2D Clustery();
}

#endif // KERNELS_H
