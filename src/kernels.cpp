#include <cmath>
#include "kernels.h"


void add_gaussian(DoubleArray2D& arr, double x, double y, double sigma, double mul = 1.0) {
	for (int j = arr.y1(); j <= arr.y2(); ++j)
		for (int i = arr.x1(); i <= arr.x2(); ++i)
			arr(i, j) += mul * exp(-0.5 * ((x-i)*(x-i)+(y-j)*(y-j)) / (sigma*sigma));
}


DoubleArray2D kernels::Determinism()
{
	DoubleArray2D kern = {
		 {  0.0,  0.0,  0.1,  3.0,  0.1,  0.0,  0.0 }
		,{  0.0,  0.1,  2.0,  0.2,  2.0,  0.1,  0.0 }
		,{  0.1,  2.0,  3.0,  0.3,  3.0,  2.0,  0.1 }
		,{  3.0,  0.2,  0.3,  0.0,  0.3,  0.2,  3.0 }
		,{  0.1,  2.0,  3.0,  0.3,  3.0,  2.0,  0.1 }
		,{  0.0,  0.1,  2.0,  0.2,  2.0,  0.1,  0.0 }
		,{  0.0,  0.0,  0.1,  3.0,  0.1,  0.0,  0.0 }
	};
	kern.shift(-3, -3);
	return kern;
}

DoubleArray2D kernels::DiagCastle()
{
	DoubleArray2D kern = {
		 {  0.0,  0.3,  0.1,  0.3,  0.1,  0.3,  0.0 }
		,{  0.3,  0.1,  0.2,  0.0,  0.2,  0.1,  0.3 }
		,{  0.1,  0.2,  0.0,  1.0,  0.0,  0.2,  0.1 }
		,{  0.3,  0.0,  1.0,  0.0,  1.0,  0.0,  0.3 }
		,{  0.1,  0.2,  0.0,  1.0,  0.0,  0.2,  0.1 }
		,{  0.3,  0.1,  0.2,  0.0,  0.2,  0.1,  0.3 }
		,{  0.0,  0.3,  0.1,  0.3,  0.1,  0.3,  0.0 }
	};
	kern.shift(-3, -3);
	return kern;
}

DoubleArray2D kernels::VerySparse()
{
	constexpr int size = 5;
	DoubleArray2D kern(-size, -size, size, size);
	add_gaussian(kern, 0, 0, 1.0, 1.0);
	return kern;
}

DoubleArray2D kernels::Clustery()
{
	DoubleArray2D kern = {
		 {  0.0,  0.0,  1.0,  0.0,  0.0 }
		,{  0.0,  0.0,  0.0,  0.0,  0.0 }
		,{  1.0,  0.0,  0.0,  0.0,  1.0 }
		,{  0.0,  0.0,  0.0,  0.0,  0.0 }
		,{  0.0,  0.0,  1.0,  0.0,  0.0 }
	};
	kern.shift(-2, -2);
	return kern;
}
