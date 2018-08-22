#include "game.h"
#include <random>
#include <utility>
#include <algorithm>
#include <QVector>


void Game::GenerateMines()
{
	QVector<MineGenerator::Point> pts = m_generator.generateUndistracted();
	
	for (int y = 0; y < m_storage.height(); ++y)
		for (int x = 0; x < m_storage.width(); ++x)
			m_storage(x, y).setHasMine(false);
	
	for (ptrdiff_t i = 0; i < m_generator.mineCount(); ++i)
		m_storage(pts[i].x, pts[i].y).setHasMine(true);
	
	m_leftover_mines = pts.mid(m_generator.mineCount());
	recomputeCounts();
}

void Game::DistractMines(int cx, int cy)
{
	QVector<MineGenerator::Point> distraction = m_generator.distraction(cx, cy);
	for (int i = 0; i < distraction.size(); ++i)
		m_leftover_mines.removeOne(distraction[i]);
	int leftover_i = 0;
	for (int i = 0; i < distraction.size(); ++i)
		if (hasMine(distraction[i].x, distraction[i].y)) {
			m_storage(distraction[i].x, distraction[i].y).setHasMine(false);
			m_storage(m_leftover_mines[leftover_i].x, m_leftover_mines[leftover_i].y).setHasMine(true);
			++leftover_i;
		}
	recomputeCounts();
}

void Game::recomputeCounts()
{
	m_unopened_count = 0;
	m_flag_count = 0;
	m_mine_count = 0;
	for (int j = 0; j < height(); ++j)
		for (int i = 0; i < width(); ++i) {
			if (!isOpen(i,j)) m_unopened_count += 1;
			if (hasFlag(i,j)) m_flag_count += 1;
			if (hasMine(i,j)) m_mine_count += 1;
		}
}

void Game::openTileImpl(QStack<Game::Point>& stack)
{
	if (stack.isEmpty()) return;
	Point p = stack.pop();
	if (isOutOfBounds(p.x, p.y)) return;
	if (isOpen(p.x, p.y) || hasFlag(p.x, p.y)) return;
	
	setIsOpen(p.x, p.y, true);
	if (hasMine(p.x, p.y)) {
		if (!isOver()) {
			m_state = Lost;
			emit gameOver();
		}
		return;
	}
	if (adjacentMines(p.x, p.y) == 0) {
		for (int j = -1; j <= 1; ++j)
			for (int i = -1; i <= 1; ++i) if (i != 0 || j != 0)
				stack.append({p.x+i, p.y+j});
	}
	if (m_mine_count == m_unopened_count) {
		if (!isOver()) {
			m_state = Won;
			emit gameOver();
		}
	}
}

Game::Game(const MineGenerator& gen, QObject* parent)
	: QObject(parent)
	, m_storage(gen.width(), gen.height())
	, m_generator(gen)
	, m_state(NotStarted)
{
	GenerateMines();
	recomputeCounts();
}

int Game::adjacentMines(int x, int y) const
{
	int ret = 0;
	for (int dy = -1; dy <= 1; ++dy)
		for (int dx = -1; dx <= 1; ++dx)
			if (hasMine(x+dx, y+dy)) ++ret;
	return ret;
}

int Game::adjacentFlags(int x, int y) const
{
	int ret = 0;
	for (int dy = -1; dy <= 1; ++dy)
		for (int dx = -1; dx <= 1; ++dx)
			if (hasFlag(x+dx, y+dy)) ++ret;
	return ret;
}

int Game::adjacentQarks(int x, int y) const
{
	int ret = 0;
	for (int dy = -1; dy <= 1; ++dy)
		for (int dx = -1; dx <= 1; ++dx)
			if (hasQark(x+dx, y+dy)) ++ret;
	return ret;
}

int Game::adjacentUnopenedTiles(int x, int y) const
{
	int ret = 0;
	for (int dy = -1; dy <= 1; ++dy)
		for (int dx = -1; dx <= 1; ++dx) {
			if (isOutOfBounds(x+dx, y+dy)) continue;
			if (!isOpen(x+dx, y+dy)) ++ret;
		}
	return ret;
}

int Game::adjacentOpenedTiles(int x, int y) const
{
	int ret = 0;
	for (int dy = -1; dy <= 1; ++dy)
		for (int dx = -1; dx <= 1; ++dx) {
			if (isOutOfBounds(x+dx, y+dy)) continue;
			if (isOpen(x+dx, y+dy)) ++ret;
		}
	return ret;
}

bool Game::isOutOfBounds(int x, int y) const
{
	return x < 0 || x >= width() || y < 0 || y >= height();
}

void Game::setIsOpen(int x, int y, bool io)
{
	if (isOpen(x, y)) m_unopened_count += 1;
	if (io)           m_unopened_count -= 1;
	setMarkState(x, y, Tile::NoMark);
	m_storage(x, y).setIsOpen(io);
}

void Game::openTile(int x, int y)
{
	if (m_state == NotStarted) {
		DistractMines(x, y);
		m_state = Running;
	}
	QStack<Point> stack;
	stack.append({x, y});
	while (!stack.isEmpty()) openTileImpl(stack);
}

void Game::setMarkState(int x, int y, int state)
{
	if (isOutOfBounds(x, y)) return;
	if (m_storage(x, y).markState() == Tile::Flag) --m_flag_count;
	m_storage(x, y).setMarkState(state);
	if (m_storage(x, y).markState() == Tile::Flag) ++m_flag_count;
}

