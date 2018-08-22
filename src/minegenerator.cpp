#include "minegenerator.h"
#include "doubleminarray2d.h"
#include <cmath>
#include <random>

MineGenerator::Point MineGenerator::maskedMinPoint(const DoubleArray2D& arr, const DoubleArray2D& mask)
{
	ptrdiff_t mx = arr.x1()-1, my = arr.y1()-1;
	for (ptrdiff_t y = arr.y1(); y <= arr.y2(); ++y)
		for (ptrdiff_t x = arr.x1(); x <= arr.x2(); ++x)
			if (mask(x, y) == 0.) {
				if (mx < arr.x1() || (arr(x, y) < arr(mx, my))) {
					mx = x;
					my = y;
				}
			}
	return {mx, my};
}

QVector<MineGenerator::Point> MineGenerator::extractPoints(QVector<MineGenerator::PDPair> pairs)
{
	QVector<MineGenerator::Point> ret;
	ret.reserve(pairs.size());
	for (ptrdiff_t i = 0; i < pairs.size(); ++i)
		ret.push_back(pairs[i].p);
	return ret;
}

DoubleArray2D MineGenerator::nrand2d(ptrdiff_t x1, ptrdiff_t y1, ptrdiff_t x2, ptrdiff_t y2, double sd, double loc)
{
	DoubleArray2D ret(x1, y1, x2, y2);
	std::random_device rd;
	std::mt19937 e2(rd());
	std::normal_distribution<double> dist(loc, sd);
	
	for (ptrdiff_t y = y1; y <= y2; ++y)
		for (ptrdiff_t x = x1; x <= x2; ++x)
			ret(x, y) = dist(e2);
	return ret;
}

QVector<MineGenerator::Point> MineGenerator::make_distraction()
{
	QVector<MineGenerator::Point> ret;
	QVector<std::pair<MineGenerator::Point, double>> tmp;
	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<double> dist(0., 1.);
	for (int j = -6; j <= 6; ++j)
		for (int i = -6; i <= 6; ++i) {
			double prob = exp(-0.2 * (i*i+j*j-2));
			if (dist(e2) <= prob) tmp.append(std::pair<MineGenerator::Point, double>({i, j}, -prob + 1e-6 * dist(e2)));
		}
	std::sort(tmp.begin(), tmp.end(), [](std::pair<MineGenerator::Point, double> l, std::pair<MineGenerator::Point, double> r){ return l.second < r.second; });
	for (int i = 0; i < tmp.size(); ++i) {
		ret.append(tmp[i].first);
	}
	return ret;
}

void MineGenerator::normalizeKernel()
{
	double sqam = 1e-30;
	for (ptrdiff_t y = m_kernel.y1(); y <= m_kernel.y2(); ++y)
		for (ptrdiff_t x = m_kernel.x1(); x <= m_kernel.x2(); ++x)
			sqam += m_kernel(x, y)*m_kernel(x, y);
	double mul = 1.0/sqrt(sqam);
	for (ptrdiff_t y = m_kernel.y1(); y <= m_kernel.y2(); ++y)
		for (ptrdiff_t x = m_kernel.x1(); x <= m_kernel.x2(); ++x)
			m_kernel(x, y) *= mul;
}

void MineGenerator::updateTotalCount()
{
	m_total_count = m_count + m_distraction.size();
	if (m_total_count > m_width * m_height) m_total_count = m_width * m_height;
}

MineGenerator::MineGenerator()
	: m_kernel({{1.}})
	, m_distraction(make_distraction())
	, m_weightfun([](ptrdiff_t x, ptrdiff_t y){ (void)x; (void)y; return 1.; })
	, m_blueness(0.)
	, m_phase(.0)
	, m_voidncluster_eps(1e-6)
	, m_width(0)
	, m_height(0)
	, m_count(0)
{
	updateTotalCount();
}

DoubleArray2D MineGenerator::bigaussianKernel(double sigma1L, double sigma1s, double angle1,
                                              double sigma2L, double sigma2s, double angle2)
{
	constexpr double twopi = 6.283185307179586476925286;
	ptrdiff_t sz = ceil(3.0 * sqrt(sigma1L*sigma1L+sigma2L*sigma2L));
	DoubleArray2D ret(-sz, -sz, sz, sz);
	
	for (ptrdiff_t y = -sz; y <= sz; ++y)
		for (ptrdiff_t x = -sz; x <= sz; ++x) {
			double L1 = ( cos(angle1)*x + sin(angle1)*y ) / sigma1L;
			double s1 = ( sin(angle1)*x - cos(angle1)*y ) / sigma1s;
			double L2 = ( cos(angle2)*x + sin(angle2)*y ) / sigma2L;
			double s2 = ( sin(angle2)*x - cos(angle2)*y ) / sigma2s;
			ret(x, y) = 
				exp( -0.5 * (L1*L1 + s1*s1) ) / sqrt(twopi * sigma1L * sigma1s) +
				exp( -0.5 * (L2*L2 + s2*s2) ) / sqrt(twopi * sigma2L * sigma2s);
		}
	return ret;
}

QVector<MineGenerator::Point> MineGenerator::voidncluster(ptrdiff_t width, ptrdiff_t height) const
{
	DoubleMinArray2D w = nrand2d(0, 0, width-1, height-1, m_voidncluster_eps, 0);
	
	QVector<MineGenerator::Point> ret;
	ret.reserve(width*height);
	for (ptrdiff_t i = 0; i < width * height; ++i) {
		DoubleMinArray2D::Point pos = w.minPoint();
		w.rawSet(pos.x, pos.y, INFINITY);
		ret.push_back({pos.x, pos.y});
		
		for (ptrdiff_t y = m_kernel.y1(); y <= m_kernel.y2(); ++y) {
			ptrdiff_t wy = y+pos.y;
			while (wy < 0) wy += height;
			while (wy >= height) wy -= height;
			for (ptrdiff_t x = m_kernel.x1(); x <= m_kernel.x2(); ++x) {
				ptrdiff_t wx = x+pos.x;
				while (wx < 0) wx += width;
				while (wx >= width) wx -= width;
				w.rawSet(wx, wy, w(wx, wy) + m_kernel(x, y));
			}
		}
		for (int dj = -1; dj <= 1; ++dj)
			for (int di = -1; di <= 1; ++di)
				w.update(pos.x + m_kernel.x1() + di*width
				        ,pos.y + m_kernel.y1() + dj*height
				        ,pos.x + m_kernel.x2() + di*width
				        ,pos.y + m_kernel.y2() + dj*height);
	}
	return ret;
}

QVector<MineGenerator::Point> MineGenerator::partlyShuffle(const QVector<MineGenerator::Point>& pts, double randomness) const
{
	std::random_device rd;
	std::mt19937 e2(rd());
	std::normal_distribution<double> dist;
	
	QVector<MineGenerator::PDPair> tmp;
	tmp.reserve(pts.size());
	for (ptrdiff_t i = 0; i < pts.size(); ++i) {
		tmp.push_back({pts[i], 
			randomness*dist(e2) + (1.0-randomness) * i * (1.0 / (m_count-1)) });
	}
	
	std::sort(tmp.begin(), tmp.end(), MineGenerator::pairCompare);
	return extractPoints(tmp);
}

QVector<MineGenerator::Point> MineGenerator::applyWeights(const QVector<MineGenerator::Point>& pts, const DoubleArray2D& w) const
{
	QVector<MineGenerator::PDPair> tmp;
	tmp.reserve(pts.size());
	for (ptrdiff_t i = 0; i < pts.size(); ++i) {
		if (pts[i].x < w.x1() || pts[i].x > w.x2() || pts[i].y < w.y1() || pts[i].y > w.y2()) continue;
		tmp.push_back({pts[i], 
			(i+1.0) / (m_count * (w(pts[i].x, pts[i].y) > 0. ? w(pts[i].x, pts[i].y) : 0.)) });
	}
	
	std::sort(tmp.begin(), tmp.end(), MineGenerator::pairCompare);
	return extractPoints(tmp);
}

QVector<MineGenerator::Point> MineGenerator::generateUndistracted() const
{
	QVector<Point> pts = voidncluster(m_width, m_height);
	pts = partlyShuffle(pts, 1. - m_blueness);
	DoubleArray2D w = DoubleArray2D(0, 0, m_width-1, m_height-1);
	for (ptrdiff_t y = 0; y < m_height; ++y)
		for (ptrdiff_t x = 0; x < m_width; ++x)
			w(x, y) = m_weightfun(x, y);
	return applyWeights(pts, w).mid(m_phase * (pts.size() - m_total_count), m_total_count);
}

QVector<MineGenerator::Point> MineGenerator::distraction(int cx, int cy) const {
	QVector<MineGenerator::Point> ret;
	for (int i = 0; i < m_total_count - m_count; ++i) {
		ret.append({m_distraction[i].x + cx, m_distraction[i].y + cy});
	}
	return ret;
}
