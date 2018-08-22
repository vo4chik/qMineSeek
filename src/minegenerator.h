#ifndef MINEGENERATOR_H
#define MINEGENERATOR_H

#include <functional>
#include <QVector>
#include "doublearray2d.h"

class MineGenerator
{
public:
	struct Point {
		ptrdiff_t x;
		ptrdiff_t y;
		bool operator==(const Point o) const { return x == o.x && y == o.y; }
	};
private:
	struct PDPair {
		Point p;
		double d;
	};
	static bool pairCompare(PDPair l, PDPair r) { return l.d < r.d; }
	
	static Point maskedMinPoint(const DoubleArray2D& arr, const DoubleArray2D& mask);
	static QVector<Point> extractPoints(QVector<PDPair> pairs);
	static DoubleArray2D nrand2d(ptrdiff_t x1, ptrdiff_t y1, ptrdiff_t x2, ptrdiff_t y2, double sd, double loc);
	static QVector<Point> make_distraction();
	void normalizeKernel();
	
	void updateTotalCount();
	
	
	DoubleArray2D m_kernel;
	QVector<Point> m_distraction;
	std::function<double(ptrdiff_t, ptrdiff_t)> m_weightfun;
	double m_blueness;
	double m_phase;
	double m_voidncluster_eps;
	
	ptrdiff_t m_width, m_height;
	ptrdiff_t m_count, m_total_count;
public:
	MineGenerator();
	
	void setSize(ptrdiff_t width, ptrdiff_t height) {
		m_width = width; m_height = height; updateTotalCount(); }
	void setWidth(ptrdiff_t width) {
		m_width = width; updateTotalCount(); }
	void setHeight(ptrdiff_t height) {
		m_height = height; updateTotalCount(); }
	ptrdiff_t width() const { return m_width; }
	ptrdiff_t height() const { return m_height; }
	
	void setMineCount(ptrdiff_t count) {
		m_count = count;
		updateTotalCount();
	}
	ptrdiff_t mineCount() const {
		return m_count; }
	
	void setKernel(const DoubleArray2D& kernel) {
		m_kernel = kernel; }
	DoubleArray2D kernel() const { return m_kernel; }
	
	void setWeightFunction(std::function<double(ptrdiff_t, ptrdiff_t)> weightfun) {
		m_weightfun = weightfun; }
	std::function<double(ptrdiff_t, ptrdiff_t)> weightFunction() const {
		return m_weightfun; }
	
	void setBlueness(double blueness) {
		m_blueness = blueness > 1. ? 1. : blueness < 0. ? 0. : blueness; }
	double blueness() const {
		return m_blueness; }
	
	
	static DoubleArray2D bigaussianKernel(double sigma1L, double sigma1s, double angle1,
	                                      double sigma2L, double sigma2s, double angle2);
	
	QVector<Point> voidncluster(ptrdiff_t width, ptrdiff_t height) const;
	QVector<Point> partlyShuffle(const QVector<Point>& pts, double randomness) const;
	QVector<Point> applyWeights(const QVector<Point>& pts, const DoubleArray2D& w) const;
	
	QVector<Point> generateUndistracted() const;
	QVector<Point> distraction(int cx, int cy) const;
};

#endif // MINEGENERATOR_H
