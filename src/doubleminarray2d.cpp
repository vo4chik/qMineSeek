#include <cmath>
#include "doubleminarray2d.h"

void DoubleMinArray2D::initialize(int width, int height)
{
	m_w = width; m_h = height;
	m_padw = m_w; if (m_padw % 4 != 0) m_padw += 4 - m_padw % 4;
	m_padh = m_h; if (m_padh % 4 != 0) m_padh += 4 - m_padh % 4;
	
	m_data = new double[m_padw*height];
	m_rows = new double[m_padh];
	
	for (int j = m_h; j < m_padh; ++j) m_rows[j] = INFINITY;
	for (int j = 0; j < height; ++j)
		for (int i = m_w; i < m_padw; ++i)
			_at(i, j) = INFINITY;
	
	m_vertical_size  = treeSizeForData (m_padh);
	m_vertical_vsize = treeVSizeForData(m_padh);
	m_horizontal_size  = treeSizeForData (m_padw);
	m_horizontal_vsize = treeVSizeForData(m_padw);
	
	m_trees = new int[m_vertical_size + height*m_horizontal_size];
	initializeTree(verticalTree(), m_vertical_vsize, m_vertical_size, height-1);
	for (int j = 0; j < height; ++j)
		initializeTree(horizontalTree(j), m_horizontal_vsize, m_horizontal_size, width-1);
}

void DoubleMinArray2D::initializeSubTree(int* tree, int l, int r, int step, int max)
{
	for (int i = l; i < r; ++i) {
		tree[i] = (i-l+1)*step - 1;
		tree[i] = tree[i] < max ? tree[i] : max;
	}
}

void DoubleMinArray2D::initializeTree(int* tree, int vsize, int size, int max)
{
	initializeSubTree(tree, vsize, size, 2, max);
	int step = 4;
	while (vsize > 1) {
		initializeSubTree(tree, vsize/2, vsize, step, max);
		step *= 2; vsize /= 2;
	}
}

int DoubleMinArray2D::treeVSizeForData(int data_size)
{
	data_size -= 1;
	int log = 0;
	while (data_size > 0) {
		data_size /= 2;
		log += 1;
	}
	return log > 0 ? 1 << (log-1) : 0;
}

int DoubleMinArray2D::treeSizeForData(int data_size)
{
	int ret = treeVSizeForData(data_size);
	ret += (data_size+1) / 2;
	return ret;
}

int* DoubleMinArray2D::horizontalTree(int j) const
{
	return m_trees + m_vertical_size + j*m_horizontal_size;
}

void DoubleMinArray2D::updateFromData(int* tree, double* data, int tree_vsize, int l, int r)
{
	for (int i = l/2; i <= r/2; ++i) {
		tree[tree_vsize + i] = data[2*i+1] < data[2*i] ? 2*i+1 : 2*i;
	}
	updateFromTree(tree, data, tree_vsize + l/2, tree_vsize + r/2);
}

void DoubleMinArray2D::updateFromTree(int* tree, double* data, int l, int r)
{
	if (r <= 1) return;
	for (int i = l/2; i <= r/2; ++i) {
		tree[i] = data[tree[2*i+1]] < data[tree[2*i]] ? tree[2*i+1] : tree[2*i];
	}
	updateFromTree(tree, data, l/2, r/2);
}

void DoubleMinArray2D::updateHorizontal(int j, int l, int r)
{
	int* tree = horizontalTree(j);
	updateFromData(tree, &_at(0, j), m_horizontal_vsize, l, r);
	m_rows[j] = _at(horizontalTree(j)[1], j);
}

void DoubleMinArray2D::updateVertical(int l, int r)
{
	updateFromData(m_trees, m_rows, m_vertical_vsize, l, r);
}

DoubleMinArray2D::DoubleMinArray2D(int width, int height, double initial_value)
{
	initialize(width, height);
	for (int j = 0; j < height; ++j)
		for (int i = 0; i < width; ++i)
			_at(i, j) = initial_value;
	update(0, 0, width-1, height-1);
}

DoubleMinArray2D::DoubleMinArray2D(const DoubleArray2D& src)
{
	initialize(src.width(), src.height());
	for (int j = 0; j < height(); ++j)
		for (int i = 0; i < width(); ++i)
			_at(i, j) = src(i, j);
	update(0, 0, width()-1, height()-1);
}

DoubleMinArray2D::~DoubleMinArray2D()
{
	delete[] m_data;
	delete[] m_rows;
	delete[] m_trees;
}

double DoubleMinArray2D::at(int x, int y) const
{
	if (x < 0 || x >= width() || y < 0 || y >= height()) return 0;
	return _at(x, y);
}

bool DoubleMinArray2D::set(int x, int y, double v)
{
	if (rawSet(x, y, v)) {
		updateHorizontal(y, x, x);
		updateVertical(y, y);
		return true;
	}
	return false;
}

bool DoubleMinArray2D::rawSet(int x, int y, double v)
{
	if (x < 0 || x >= m_w || y < 0 || y >= m_h) return false;
	_at(x, y) = v;
	return true;
}

void DoubleMinArray2D::update(int x1, int y1, int x2, int y2)
{
	if (x2 < x1) return;
	if (y2 < y1) return;
	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (x2 >= width())  x2 = width() - 1;
	if (y2 >= height()) y2 = height()- 1;
	for (int j = y1; j <= y2; ++j)
		updateHorizontal(j, x1, x2);
	updateVertical(y1, y2);
}

double DoubleMinArray2D::min() const
{
	Point p = minPoint();
	return _at(p.x, p.y);
}

DoubleMinArray2D::Point DoubleMinArray2D::minPoint() const
{
	int y = m_trees[1];
	return { horizontalTree(y)[1], y };
}
