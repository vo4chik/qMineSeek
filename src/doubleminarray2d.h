#ifndef DOUBLEMINARRAY2D_H
#define DOUBLEMINARRAY2D_H

#include "doublearray2d.h"

class DoubleMinArray2D
{
public:
	struct Point {
		int x, y;
	};
private:
	double* m_data;
	double* m_rows;
	int* m_trees;
	
	int m_w, m_h;
	int m_padw, m_padh;
	int m_vertical_size, m_vertical_vsize;
	int m_horizontal_size, m_horizontal_vsize;
	
	void initialize(int width, int height);
	void initializeSubTree(int* tree, int l, int r, int step, int max);
	void initializeTree(int* tree, int vsize, int size, int max);
	
	static int treeVSizeForData(int data_size);
	static int treeSizeForData(int data_size);
	
	int* horizontalTree(int j) const;
	int* verticalTree() const { return m_trees; }
	
	void updateFromData(int* tree, double* data, int tree_vsize, int l, int r);
	void updateFromTree(int* tree, double* data, int l, int r);
	
	void updateHorizontal(int j, int l, int r);
	void updateVertical(int l, int r);
	
	double& _at(int x, int y) { return m_data[x + y*m_padw]; }
	const double& _at(int x, int y) const { return m_data[x + y*m_padw]; }
public:
	DoubleMinArray2D(int width, int height, double initial_value = 0.0);
	DoubleMinArray2D(const DoubleArray2D& src);
	~DoubleMinArray2D();
	
	int width() const { return m_w; }
	int height() const { return m_h; }
	
	double at(int x, int y) const;
	double operator()(int x, int y) const { return at(x, y); }
	bool set(int x, int y, double v);
	
	bool rawSet(int x, int y, double v);
	void update(int x1, int y1, int x2, int y2);
	
	double min() const;
	Point minPoint() const;
};

#endif // DOUBLEMINARRAY2D_H
