#ifndef TILESWIDGET_H
#define TILESWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QPainter>
#include <QMouseEvent>
#include "tilescolorscheme.h"
#include "settings.h"
#include "game.h"

class TilesWidget : public QWidget
{
	Q_OBJECT
private:
	struct TileDescription {
		TilesColorscheme::Pair colors;
		QString text;
	};
	
	constexpr static int CFMouse1 = 1;
	constexpr static int CFMouse2 = 2;
	constexpr static int CFMouse3 = 4;
	constexpr static int CFCtrl   = 8;
	constexpr static int CFShift  = 16;
	
	QPoint m_position;
	QPoint m_mouse_position;
	int m_tile_size, m_font_size, m_tile_padding;
	double m_tile_size_f;
	
	QPoint m_hovered_tile;
	QPoint m_pressed_tile;
	int m_pending_click_flags;
	
	Game* m_game;
	Settings* m_settings;
	
	QRect tileRectAbsolute(int x, int y);
	QPoint LUTileAtAbsolute(int x, int y);
	QPoint LUTile();
	QPoint RDTile();
	
	TileDescription tileDescription(int x, int y, const TilesColorscheme& colorscheme);
	void drawTile(QPainter& p, int x, int y, const TilesColorscheme& colorscheme);
	
	void clicked(int x, int y, int flags);
	
	void clampCenterPosition();
	void relativeZoom(int x, int y, double factor);
	void receiveMouseEvent(QMouseEvent* e);
	void fullRedraw();
private slots:
	void mouseMoveEvent(QMouseEvent* e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void leaveEvent(QEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
public:
	explicit TilesWidget(Game* game, Settings* settings, QWidget *parent = nullptr);
	
	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;
	
	void setTileSize(int size);
	int tileSize() const { return m_tile_size; }
	
	Game* game() const { return m_game; }
	void setGame(Game* game);
signals:
	void updated();
public slots:
};

#endif // TILESWIDGET_H
