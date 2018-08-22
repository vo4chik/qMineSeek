#ifndef TILESCOLORSCHEME_H
#define TILESCOLORSCHEME_H

#include <QColor>

class TilesColorscheme
{
public:
	struct Pair {
		QColor fg;
		QColor bg;
	};
	struct Cell {
		Pair plain;
		Pair hovered;
		Pair pressed;
		Pair revealedT;
		Pair revealedF;
		Pair adjacent;
		//Pair adjpressed;
	};
	
	QColor background;
	Cell unopened;
	Cell flag;
	Cell qark;
	
	Pair openedPlain;
	Pair openedFinished;
	Pair openedError;
	Pair openedQarked;
	Pair openedQarkedFinished;
	Pair openedQarkedError;
	Pair openedMine;
public:
	static TilesColorscheme Dark();
	static TilesColorscheme Light();
};

#endif // TILESCOLORSCHEME_H
