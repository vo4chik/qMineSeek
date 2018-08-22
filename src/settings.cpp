#include <QSettings>
#include "settings.h"

void Settings::update()
{
	if (!needs_update) return;
	s_kernel = 
		_kernel == Determinism ? kernels::Determinism() :
		_kernel == DiagCastle  ? kernels::DiagCastle()  :
		_kernel == VerySparse  ? kernels::VerySparse()  :
		_kernel == Clustery    ? kernels::Clustery()    :
		                         DoubleArray2D(0, 0, 0, 0);
	s_colorscheme = 
		  _colorscheme == Dark        ? TilesColorscheme::Dark()
		:                               TilesColorscheme::Light();
	needs_update = false;
}

void Settings::setSizesPreset(SizesPreset preset)
{
	_sizespreset = preset;
	if (preset == Easy) {
		_w = 9; _h = 9;
		_cnt = 10;
	}
	if (preset == Medium) {
		_w = 16; _h = 16;
		_cnt = 40;
	}
	if (preset == Hard) {
		_w = 30; _h = 16;
		_cnt = 99;
	}
	if (preset == Large) {
		_w = 99; _h = 99;
		_cnt = 999;
	}
}

Settings::Settings()
{
	setSizesPreset(Easy);
	
	setRelativeMineCounts(false);
	setMiddleClickPlacesFlags(false);
	setRightClickAsMiddle(false);
	setBlueness(0.2);
	
	setColorscheme(Light);
	setKernel(Determinism);
	
	needs_update = true;
}

void Settings::applyToGenerator(MineGenerator* gen)
{
	gen->setSize(width(), height());
	gen->setMineCount(mineCount());
	
	gen->setBlueness(blueness());
	gen->setKernel(getKernel());
}

void Settings::save()
{
	QSettings s;
	s.setValue(QString("game/width"), width());
	s.setValue(QString("game/height"), height());
	s.setValue(QString("game/minecount"), mineCount());
	s.setValue(QString("game/relativeCounts"), relativeMineCounts());
	s.setValue(QString("game/middleFlags"), middleClickPlacesFlags());
	s.setValue(QString("game/rightAsMiddle"), rightClickAsMiddle());
	s.setValue(QString("game/blueness"), blueness());
	s.setValue(QString("game/colorscheme"), colorscheme());
	s.setValue(QString("game/kernel"), kernel());
}

void Settings::load()
{
	QSettings s;
	if (s.contains(QString("game/width")))
		setWidth(s.value(QString("game/width")).toInt());
	if (s.contains(QString("game/height")))
		setHeight(s.value(QString("game/height")).toInt());
	if (s.contains(QString("game/minecount")))
		setMineCount(s.value(QString("game/minecount")).toInt());
	if (s.contains(QString("game/relativeCounts")))
		setRelativeMineCounts(s.value(QString("game/relativeCounts")).toBool());
	if (s.contains(QString("game/middleFlags")))
		setMiddleClickPlacesFlags(s.value(QString("game/middleFlags")).toBool());
	if (s.contains(QString("game/rightAsMiddle")))
		setRightClickAsMiddle(s.value(QString("game/rightAsMiddle")).toBool());
	if (s.contains(QString("game/blueness")))
		setBlueness(s.value(QString("game/blueness")).toDouble());
	if (s.contains(QString("game/colorscheme")))
		setColorscheme(Colorscheme(s.value(QString("game/colorscheme")).toInt()));
	if (s.contains(QString("game/kernel")))
		setKernel(Kernel(s.value(QString("game/kernel")).toInt()));
	if (_w ==  9 && _h ==  9 && _cnt ==  10) setSizesPreset(Easy);
	if (_w == 16 && _h == 16 && _cnt ==  40) setSizesPreset(Medium);
	if (_w == 30 && _h == 16 && _cnt ==  99) setSizesPreset(Hard);
	if (_w == 99 && _h == 99 && _cnt == 999) setSizesPreset(Large);
}
