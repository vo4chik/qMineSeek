#ifndef SETTINGS_H
#define SETTINGS_H

#include <QStringList>
#include "tilescolorscheme.h"
#include "minegenerator.h"
#include "kernels.h"

class Settings
{
public:
	enum Colorscheme {
		 Dark
		,Light
	};
	static QStringList colorschemeList() {
		return { QString("Dark"), QString("Light") };
	}
	
	enum Kernel {
		 Determinism
		,DiagCastle
		,VerySparse
		,Clustery
	};
	static QStringList kernelList() {
		return { QString("Determinism")
		       , QString("DiagCastle")
		       , QString("VerySparse")
		       , QString("Clustery") };
	}
	
	enum SizesPreset {
		 Easy
		,Medium
		,Hard
		,Large
		,Custom
	};
	static QStringList sizesPresetList() {
		return { QString("Easy")
		       , QString("Medium")
		       , QString("Hard")
		       , QString("Large")
		       , QString("Custom") };
	}
private:
	int _w, _h, _cnt;
	
	bool _relativeMineCounts;
	bool _middleClickPlacesFlags;
	bool _rightAsMiddle;
	double _blueness;
	
	SizesPreset _sizespreset;
	Colorscheme _colorscheme;
	Kernel _kernel;
	
	bool needs_update;
	TilesColorscheme s_colorscheme;
	DoubleArray2D s_kernel;
	
	void update();
public:
	int width() const { return _w; }
	int height() const { return _h; }
	int mineCount() const { return _cnt; }
	
	void setWidth(int width) { if (width != _w) { _sizespreset = Custom; _w = width; } }
	void setHeight(int height) { if (height != _h) { _sizespreset = Custom; _h = height; } }
	void setSize(int width, int height) { setWidth(width); setHeight(height); }
	void setMineCount(int count) { if (count != _cnt) { _sizespreset = Custom; _cnt = count; } }
	
	bool relativeMineCounts() const { return _relativeMineCounts; }
	void setRelativeMineCounts(bool f) { _relativeMineCounts = f; }
	
	bool middleClickPlacesFlags() const { return _middleClickPlacesFlags; }
	void setMiddleClickPlacesFlags(bool f) { _middleClickPlacesFlags = f; }
	
	bool rightClickAsMiddle() const { return _rightAsMiddle; }
	void setRightClickAsMiddle(bool f) { _rightAsMiddle = f; }
	
	double blueness() const { return _blueness; }
	void setBlueness(double blueness) { _blueness = blueness; }
	
	SizesPreset sizesPreset() const { return _sizespreset; }
	void setSizesPreset(SizesPreset preset);
	
	Colorscheme colorscheme() const { return _colorscheme; }
	void setColorscheme(Colorscheme colorscheme) { needs_update = true; _colorscheme = colorscheme; }
	TilesColorscheme getColorscheme() { update(); return s_colorscheme; }
	
	Kernel kernel() const { return _kernel; }
	void setKernel(Kernel kern) { needs_update = true; _kernel = kern; }
	DoubleArray2D getKernel() { update(); return s_kernel; }
	
	Settings();
	Settings(const Settings& o)
		: _w(o._w), _h(o._h), _cnt(o._cnt)
		, _relativeMineCounts(o._relativeMineCounts)
		, _middleClickPlacesFlags(o._middleClickPlacesFlags)
		, _rightAsMiddle(o._rightAsMiddle)
		, _blueness(o._blueness)
		, _sizespreset(o._sizespreset)
		, _colorscheme(o._colorscheme)
		, _kernel(o._kernel)
		, needs_update(true) {}
	
	void applyToGenerator(MineGenerator* gen);
	void save();
	void load();
};

#endif // SETTINGS_H
