#ifndef TILESTORAGE_TPP
#define TILESTORAGE_TPP

#include "tilestorage.h"


TileStorage::TileStorage(int width, int height)
	: m_width(width), m_height(height)
	, m_data(new Tile[width*height])
{
}

TileStorage::~TileStorage()
{
	delete[] m_data;
}

Tile &TileStorage::operator()(int x, int y)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) return m_oob_tile;
	return m_data[x + y*m_width];
}

const Tile&TileStorage::operator()(int x, int y) const
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) return m_oob_tile;
	return m_data[x + y*m_width];
}

#endif //TILESTORAGE_TPP
