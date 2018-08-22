#include <cmath>
#include "tilescolorscheme.h"


QColor hsl(double h, double s, double l) {
	return QColor::fromHslF(h, s, l);
}

QColor mix(QColor a, QColor b, double m = 0.5) {
	qreal ha, sa, la;
	qreal hb, sb, lb;
	a.getHslF(&ha, &sa, &la);
	b.getHslF(&hb, &sb, &lb);
	constexpr double twopi = 6.28318530717958647688;
	double x = m * cos(twopi*ha) * sa + (1 - m) * cos(twopi*hb) * sb;
	double y = m * sin(twopi*ha) * sa + (1 - m) * sin(twopi*hb) * sb;
	double reth = atan2(y, x) / twopi;
	if (reth < 0.) reth += 1;
	return QColor::fromHslF(
		 reth
		,m * sa + (1. - m) * sb
		,m * la + (1. - m) * lb);
}



TilesColorscheme TilesColorscheme::Dark()
{
	TilesColorscheme ret;
	
	ret.background = hsl(0.70, 0.05, 0.10);
	
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
	QColor tile   = hsl(0.70, 0.05, 0.40);
	QColor tileL  = hsl(0.70, 0.05, 0.45);
	QColor tileLL = hsl(0.70, 0.05, 0.58);
	QColor tileL4 = hsl(0.70, 0.05, 0.80);
	QColor tileD  = hsl(0.70, 0.05, 0.30);
	QColor tileDD = hsl(0.70, 0.05, 0.20);
	QColor tileD3 = hsl(0.70, 0.05, 0.10);
	QColor white  = hsl(0.20, 0.05, 0.60);
	
	QColor red      = hsl(0.00, 0.60, 0.40);
	QColor redD     = hsl(0.00, 0.60, 0.20);
	QColor redDD    = hsl(0.00, 0.60, 0.10);
	QColor green    = hsl(0.35, 0.60, 0.35);
	QColor greenDD  = hsl(0.35, 0.60, 0.10);
	QColor yellow   = hsl(0.13, 0.60, 0.40);
	QColor yellowDD = hsl(0.13, 0.60, 0.10);
	QColor orange   = hsl(0.08, 0.60, 0.40);
	QColor orangeL  = hsl(0.08, 0.60, 0.45);
	QColor orangeLL = hsl(0.08, 0.60, 0.58);
	QColor orangeDD = hsl(0.08, 0.60, 0.10);
	QColor blue     = hsl(0.66, 0.50, 0.40);
	QColor blueDD   = hsl(0.66, 0.50, 0.10);
	QColor purple   = hsl(0.80, 0.40, 0.40);
	QColor purpleL  = hsl(0.80, 0.40, 0.45);
	QColor purpleLL = hsl(0.80, 0.40, 0.58);
	QColor purpleDD = hsl(0.80, 0.40, 0.10);
#pragma GCC diagnostic pop
	
	ret.unopened.plain     = { tileL4 , tile };
	ret.unopened.hovered   = { tileL4 , tileL };
	ret.unopened.pressed   = { tileL4 , tileLL };
	ret.unopened.revealedT = { greenDD, green };
	ret.unopened.revealedF = { redDD  , red };
	ret.unopened.adjacent  = { blueDD , blue };
	
	ret.flag.plain     = { tileD3  , tile };
	ret.flag.revealedT = { greenDD , green };
	ret.flag.revealedF = { redDD   , red };
	ret.flag.adjacent  = { purpleDD, purple };
	ret.flag.hovered   = { purpleDD, purpleL };
	ret.flag.pressed   = { purpleDD, purpleLL };
	
	ret.qark.plain     = { yellowDD, yellow };
	ret.qark.revealedT = { greenDD , green };
	ret.qark.revealedF = { redDD   , red };
	ret.qark.adjacent  = { orangeDD, purple };
	ret.qark.hovered   = { orangeDD, orangeL };
	ret.qark.pressed   = { orangeDD, orangeLL };
	
	ret.openedPlain          = { white  , tileDD };
	ret.openedFinished       = { tileD  , tileDD };
	ret.openedError          = { red    , redDD };
	ret.openedQarked         = { yellow , yellowDD };
	ret.openedQarkedFinished = { green  , yellowDD };
	ret.openedQarkedError    = { red    , yellowDD };
	ret.openedMine           = { red    , tileDD };
	
	return ret;
}

TilesColorscheme TilesColorscheme::Light()
{
	TilesColorscheme ret;
	
	ret.background = hsl(0.13, 0.08, 0.90);
	
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
	QColor tile   = hsl(0.13, 0.07, 0.45);
	QColor tileD  = hsl(0.13, 0.06, 0.32);
	QColor tileDD = hsl(0.13, 0.04, 0.24);
	QColor tileD4 = hsl(0.13, 0.03, 0.16);
	QColor tileL  = hsl(0.13, 0.08, 0.56);
	QColor tileLL = hsl(0.13, 0.09, 0.68);
	QColor tileL3 = hsl(0.13, 0.11, 0.80);
	QColor black  = hsl(0.60, 0.00, 0.25);
	
	QColor red      = hsl(0.00, 0.60, 0.50);
	QColor redL     = hsl(0.00, 0.60, 0.56);
	QColor redLL    = hsl(0.00, 0.60, 0.80);
	QColor green    = hsl(0.35, 0.60, 0.40);
	QColor greenLL  = hsl(0.35, 0.60, 0.80);
	QColor yellow   = hsl(0.13, 0.60, 0.45);
	QColor yellowLL = hsl(0.13, 0.60, 0.80);
	QColor orange   = hsl(0.08, 0.60, 0.60);
	QColor orangeD  = hsl(0.08, 0.60, 0.53);
	QColor orangeDD = hsl(0.08, 0.60, 0.44);
	QColor orangeLL = hsl(0.08, 0.60, 0.80);
	QColor blue     = hsl(0.66, 0.50, 0.45);
	QColor blueLL   = hsl(0.66, 0.50, 0.80);
	QColor purple   = hsl(0.80, 0.50, 0.50);
	QColor purpleD  = hsl(0.80, 0.55, 0.45);
	QColor purpleDD = hsl(0.80, 0.60, 0.40);
	QColor purpleLL = hsl(0.80, 0.50, 0.80);
#pragma GCC diagnostic pop
	
	ret.unopened.plain     = { tileD4 , tile };
	ret.unopened.hovered   = { tileD4 , tileD };
	ret.unopened.pressed   = { tileD4 , tileDD };
	ret.unopened.revealedT = { greenLL, green };
	ret.unopened.revealedF = { redLL  , red };
	ret.unopened.adjacent  = { blueLL , blue };
	
	ret.flag.plain     = { tileL3  , tile };
	ret.flag.revealedT = { greenLL , green };
	ret.flag.revealedF = { redLL   , red };
	ret.flag.adjacent  = { purpleLL, purple };
	ret.flag.hovered   = { purpleLL, purpleD };
	ret.flag.pressed   = { purpleLL, purpleDD };
	
	ret.qark.plain     = { yellowLL, yellow };
	ret.qark.revealedT = { greenLL , green };
	ret.qark.revealedF = { redLL   , red };
	ret.qark.adjacent  = { orangeLL, purple };
	ret.qark.hovered   = { orangeLL, orangeD };
	ret.qark.pressed   = { orangeLL, orangeDD };
	
	ret.openedPlain          = { black  , tileLL };
	ret.openedFinished       = { tileL  , tileLL };
	ret.openedError          = { red    , redLL };
	ret.openedQarked         = { yellow , yellowLL };
	ret.openedQarkedFinished = { green  , yellowLL };
	ret.openedQarkedError    = { red    , yellowLL };
	ret.openedMine           = { red    , tileLL };
	
	return ret;
}

