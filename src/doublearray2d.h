#ifndef DOUBLEARRAY2D_H
#define DOUBLEARRAY2D_H

#include <utility>
#include <cstddef>

class DoubleArray2D
{
public:
	using size_type = ptrdiff_t;
private:
	double* m_data;
	size_type m_x1, m_y1;
	size_type m_w, m_h;
public:
	DoubleArray2D();
	DoubleArray2D(size_type x1, size_type y1, size_type x2, size_type y2);
	DoubleArray2D(size_type width, size_type height): DoubleArray2D(0, 0, width-1, height-1) {}
	~DoubleArray2D();
	
	DoubleArray2D(std::initializer_list<std::initializer_list<double>> list);
	
	DoubleArray2D(const DoubleArray2D& other);
	DoubleArray2D& operator= (const DoubleArray2D& other);
	
	double& operator()(size_type x, size_type y);
	double& operator()(size_type x, size_type y) const;
	
	size_type width() const { return m_w; }
	size_type height() const { return m_h; }
	size_type x1() const { return m_x1; }
	size_type y1() const { return m_y1; }
	size_type x2() const { return m_w + m_x1 - 1; }
	size_type y2() const { return m_h + m_y1 - 1; }
	
	void shift(size_type dx, size_type dy) { m_x1 += dx; m_y1 += dy; }
	void setPos1(size_type x1, size_type y1) { m_x1 = x1; m_y1 = y1; }
	
	bool isValidCoords(size_type x, size_type y) const {
		return x >= x1() && y >= y1() && x <= x2() && y <= y2(); }
};

#endif // DOUBLEARRAY2D_H
