#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QStack>

#include "tile.h"
#include "tilestorage.h"
#include "doublearray2d.h"
#include "minegenerator.h"

class Game: public QObject
{
	Q_OBJECT
	
public:
	enum State {
		 NotStarted
		,Running
		,Lost
		,Won
	};
private:
	struct Point {
		int x, y;
	};
	
	TileStorage m_storage;
	int m_mine_count;
	
	MineGenerator m_generator;
	
	int m_flag_count;
	int m_unopened_count;
	State m_state;
	
	QVector<MineGenerator::Point> m_leftover_mines;
	void GenerateMines();
	void DistractMines(int cx, int cy);
	
	void recomputeCounts();
	void openTileImpl(QStack<Point>& stack);
	
	void mk_kernels();
public:
	Game(const MineGenerator& gen, QObject* parent = nullptr);
	
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	
	int width() const { return m_storage.width(); }
	int height() const { return m_storage.height(); }
	
	Tile& operator()(int x, int y) { return m_storage(x, y); }
	const Tile& operator()(int x, int y) const { return m_storage(x, y); }
	Tile& at(int x, int y) { return m_storage(x, y); }
	const Tile& at(int x, int y) const { return m_storage(x, y); }
	
	int adjacentMines(int x, int y) const;
	int adjacentFlags(int x, int y) const;
	int adjacentQarks(int x, int y) const;
	int adjacentUnopenedTiles(int x, int y) const;
	int adjacentOpenedTiles(int x, int y) const;
	
	bool isOutOfBounds(int x, int y) const;
	
	bool hasMine(int x, int y) const { return at(x, y).hasMine(); }
	bool hasFlag(int x, int y) const { return at(x, y).hasFlag(); }
	bool hasQark(int x, int y) const { return at(x, y).hasQark(); }
	bool hasMark(int x, int y) const { return at(x, y).hasMark(); }
	
	bool isOpen(int x, int y) const { return at(x, y).isOpen(); }
	void setIsOpen(int x, int y, bool io);
	
	int markState(int x, int y) const { return at(x, y).markState(); }
	void setMarkState(int x, int y, int state);
	
	int mineCount() const { return m_mine_count; }
	int openedCount() const { return width() * height() - m_unopened_count; }
	int unopenedCount() const { return m_unopened_count; }
	int seeminglyUnfoundMineCount() const { return m_mine_count - m_flag_count; }
	
	void openTile(int x, int y);
	
	State state() const { return m_state; }
	bool isOver() const { return m_state == Won || m_state == Lost; }
	bool isWon() const { return m_state == Won; }
	bool isLost() const { return m_state == Lost; }
public slots:
	
signals:
	void gameOver();
};

#endif // GAME_H
