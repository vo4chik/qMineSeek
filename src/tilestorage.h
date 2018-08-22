#ifndef TILESTORAGE_H
#define TILESTORAGE_H

#include "tile.h"
//#include <cstdint>
#include <cstddef>

class TileStorage
{
	int m_width;
	int m_height;
	
	Tile* m_data;
	Tile m_oob_tile;
public:
	TileStorage(int width, int height);
	~TileStorage();
	
	int width() const { return m_width; }
	int height() const { return m_height; }
	
	TileStorage(const TileStorage&) = delete;
	Tile& operator=(const TileStorage&) = delete;
	
	Tile& operator() (int x, int y);
	const Tile& operator()(int x, int y) const;
};

#endif // TILESTORAGE_H
