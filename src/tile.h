#ifndef TILE_H
#define TILE_H


class Tile
{
	bool m_hasmine;
	bool m_isopen;
	int m_markstate;
public:
	constexpr static int NoMark = 0;
	constexpr static int Flag = 1;
	constexpr static int QuestionMark = 2;
	
	Tile(): m_hasmine(false), m_isopen(false), m_markstate(NoMark) {}
	
	bool hasMine() const { return m_hasmine; }
	void setHasMine(bool new_hasMine) { m_hasmine = new_hasMine; }
	
	bool isOpen() const { return m_isopen; }
	void setIsOpen(bool new_isOpen) { m_isopen = new_isOpen; if (m_isopen) m_markstate = Tile::NoMark; }
	
	int markState() const { return m_markstate; }
	void setMarkState(int new_markstate) { m_markstate = m_isopen ? Tile::NoMark : new_markstate; }
	
	bool hasMark() const { return m_markstate != Tile::NoMark; }
	bool hasFlag() const { return m_markstate == Tile::Flag; }
	bool hasQark() const { return m_markstate == Tile::QuestionMark; }
};

#endif // TILE_H
