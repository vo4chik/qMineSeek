#include "doublearray2d.h"

DoubleArray2D::DoubleArray2D()
	: m_data(nullptr), m_x1(0), m_y1(0), m_w(0), m_h(0)
{}

DoubleArray2D::DoubleArray2D(DoubleArray2D::size_type x1, DoubleArray2D::size_type y1, DoubleArray2D::size_type x2, DoubleArray2D::size_type y2)
{
	m_x1 = x1;
	m_y1 = y1;
	m_w = x2 - x1 + 1; if (m_w < 0) m_w = 0;
	m_h = y2 - y1 + 1; if (m_h < 0) m_h = 0;
	m_data = new double[m_w*m_h];
	for (size_type i = 0; i < m_w*m_h; ++i)
		m_data[i] = 0;
}

DoubleArray2D::~DoubleArray2D()
{
	delete[] m_data;
}

DoubleArray2D::DoubleArray2D(std::initializer_list<std::initializer_list<double> > list)
{
	m_x1 = 0;
	m_y1 = 0;
	m_h = list.size();
	m_w = 0;
	for (auto it = list.begin(); it < list.end(); ++it)
		if ((ptrdiff_t)it->size() > m_w) m_w = it->size();
	m_data = new double[m_w*m_h];
	for (size_type i = 0; i < m_w*m_h; ++i)
		m_data[i] = 0;
	for (struct { size_type y; decltype(list.begin()) it; } y = {0, list.begin()}; y.it < list.end(); ++y.y, ++y.it)
		for (struct { size_type x; const double* it; } x = {0, y.it->begin()}; x.it < y.it->end(); ++x.x, ++x.it)
			(*this)(x.x, y.y) = *x.it;
}

DoubleArray2D::DoubleArray2D(const DoubleArray2D& other)
{
	m_x1 = other.x1();
	m_y1 = other.y1();
	m_w = other.width();
	m_h = other.height();
	m_data = new double[m_w*m_h];
	for (size_type i = 0; i < m_w*m_h; ++i)
		m_data[i] = other.m_data[i];
}

DoubleArray2D& DoubleArray2D::operator=(const DoubleArray2D& other)
{
	if (this == &other) return *this;
	delete[] m_data;
	m_x1 = other.x1();
	m_y1 = other.y1();
	m_w = other.width();
	m_h = other.height();
	m_data = new double[m_w*m_h];
	for (size_type i = 0; i < m_w*m_h; ++i)
		m_data[i] = other.m_data[i];
	return *this;
}

double &DoubleArray2D::operator()(DoubleArray2D::size_type x, DoubleArray2D::size_type y)
{
	return m_data[(x-m_x1) + (y-m_y1)*m_w];
}

double&DoubleArray2D::operator()(DoubleArray2D::size_type x, DoubleArray2D::size_type y) const
{
	return m_data[(x-m_x1) + (y-m_y1)*m_w];
}
