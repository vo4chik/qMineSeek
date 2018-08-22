#include <cmath>
#include <QApplication>
#include "tileswidget.h"

int floordiv(int x, int y) {
	return (x / y) - (x % y < 0 ? 1 : 0);
}

QRect TilesWidget::tileRectAbsolute(int x, int y)
{
	return QRect(x*(m_tile_size+m_tile_padding) + m_tile_padding,
	             y*(m_tile_size+m_tile_padding) + m_tile_padding,
	             m_tile_size, m_tile_size);
}

QPoint TilesWidget::LUTileAtAbsolute(int x, int y)
{
	int div = m_tile_size + m_tile_padding;
	return QPoint(floordiv(x - m_tile_padding, div)
	             ,floordiv(y - m_tile_padding, div));
}

QPoint TilesWidget::LUTile()
{
	QPoint ret = LUTileAtAbsolute(-m_position.x()+m_tile_padding, -m_position.y()+m_tile_padding);
	if (ret.x() < 0) ret.setX(0);
	if (ret.y() < 0) ret.setY(0);
	return ret;
}

QPoint TilesWidget::RDTile()
{
	QPoint ret = LUTileAtAbsolute(-m_position.x()+width()-1, -m_position.y()+height()-1);
	if (ret.x() >= m_game->width() ) ret.setX(m_game->width() -1);
	if (ret.y() >= m_game->height()) ret.setY(m_game->height()-1);
	return ret;
}

TilesWidget::TileDescription TilesWidget::tileDescription(int x, int y, const TilesColorscheme& colorscheme)
{
	TilesWidget::TileDescription ret;
	if (m_game->isOpen(x, y)) {
		if (m_game->hasMine(x, y)) {
			ret.text = QString("*");
			ret.colors = colorscheme.openedMine;
			return ret;
		}
		ret.colors = colorscheme.openedPlain;
		int adjm = m_game->adjacentMines(x, y);
		if (adjm == 0) return ret;
		int adjf = m_game->adjacentFlags(x, y);
		int adjq = m_game->adjacentQarks(x, y);
		ret.text = QString::number(adjm - (m_settings->relativeMineCounts() ? adjf + adjq : 0));
		if (adjf > adjm) {
			ret.colors = colorscheme.openedError;
			return ret;
		}
		if (adjq > 0) {
			if (adjf + adjq == adjm)
				ret.colors = colorscheme.openedQarkedFinished;
			else if (adjf + adjq > adjm)
				ret.colors = colorscheme.openedQarkedError;
			else
				ret.colors = colorscheme.openedQarked;
			return ret;
		}
		int adju = m_game->adjacentUnopenedTiles(x, y);
		if (adju == adjm && adjf == adjm) {
			ret.colors = colorscheme.openedFinished;
			return ret;
		}
		return ret;
	}
	const TilesColorscheme::Cell* cell =
		  m_game->markState(x, y) == Tile::QuestionMark ? &colorscheme.qark
		: m_game->markState(x, y) == Tile::Flag         ? &colorscheme.flag
	                                                    : &colorscheme.unopened;
	if (m_game->isOver() && !m_game->hasMark(x, y)) {
		ret.text = m_game->hasMine(x, y) ? QString("*") : QString("");
		ret.colors = !m_game->hasMine(x, y) ? cell->plain
		           :  m_game->isWon()       ? cell->revealedT : cell->revealedF;
		return ret;
	}
	QPoint diff = m_hovered_tile - QPoint(x, y);
	bool isNear = diff.x() >= -1 && diff.x() <= 1 && diff.y() >= -1 && diff.y() <= 1;
	if (isNear && !m_game->hasFlag(x, y) && m_pending_click_flags & CFMouse3
	           && !(m_pending_click_flags & CFMouse2 && diff == QPoint(0, 0))) {
		ret.colors = colorscheme.openedPlain;
		return ret;
	}
	if (m_game->hasMark(x, y))
		ret.text = m_game->hasFlag(x, y) ? QString("F") : QString("?");
	if (m_game->isOver() && m_game->hasMark(x, y)) {
		ret.colors = m_game->hasMine(x, y) ? cell->revealedT : cell->revealedF;
		return ret;
	}
	bool mouse1or2 = m_pending_click_flags & (CFMouse1 + CFMouse2);
	bool notJust3 = !(m_pending_click_flags & CFMouse3) || mouse1or2;
	bool isAdjacent = isNear && (m_game->isOpen(m_hovered_tile.x(), m_hovered_tile.y())
	                         ||  m_game->hasMark(x, y));
	bool isPressed = m_pressed_tile == QPoint(x, y) && m_pressed_tile == m_hovered_tile && mouse1or2;
	bool isHovered = notJust3 &&
	                 m_pressed_tile.x() >= 0 ? m_pressed_tile == QPoint(x, y)
	                                         : m_hovered_tile == QPoint(x, y);
	ret.colors = isPressed  ? cell->pressed
	           : isHovered  ? cell->hovered
	           : isAdjacent ? cell->adjacent
	           :              cell->plain;
	return ret;
}


void TilesWidget::drawTile(QPainter& p, int x, int y, const TilesColorscheme& colorscheme)
{
	if (x < 0 || y < 0 || x >= m_game->width() || y >= m_game->height()) return;
	QRect rect = tileRectAbsolute(x, y);
	rect.translate(m_position);
	TileDescription desc = tileDescription(x, y, colorscheme);
	p.fillRect(rect, QBrush(desc.colors.bg));
	if (m_font_size > 0) {
		p.setPen(desc.colors.fg);
		p.drawText(rect, Qt::AlignCenter, desc.text);
	}
}

void TilesWidget::clicked(int x, int y, int flags)
{
	if (m_game->isOver()) return;
	bool isAtPressed = m_hovered_tile == m_pressed_tile;
	if (flags & CFMouse1 && isAtPressed) {
		if (!(flags & CFCtrl))
			m_game->openTile(x, y);
	}
	if (flags & CFMouse2 && isAtPressed) {
		int target = flags & CFCtrl ? Tile::QuestionMark : Tile::Flag;
		m_game->setMarkState(x, y, m_game->hasMark(x, y) ? Tile::NoMark : target);
	}
	if (flags & CFMouse3) {
		if (!m_game->isOpen(x, y)) return;
		int adjm = m_game->adjacentMines(x, y);
		int adjf = m_game->adjacentFlags(x, y);
		int adjo = m_game->adjacentUnopenedTiles(x, y);
		if (adjm == adjo && m_settings->middleClickPlacesFlags()) {
			for (int dy = -1; dy <= 1; ++dy)
				for (int dx = -1; dx <= 1; ++dx)
					if (!m_game->isOpen(x+dx, y+dy))
						m_game->setMarkState(x+dx, y+dy, Tile::Flag);
			return;
		}
		if (adjm == adjf) {
			for (int dy = -1; dy <= 1; ++dy)
				for (int dx = -1; dx <= 1; ++dx)
					if (!m_game->isOpen(x+dx, y+dy))
						m_game->openTile(x+dx, y+dy);
			return;
		}
	}
}

void TilesWidget::clampCenterPosition()
{
	int size_x = m_game->width()  * (m_tile_size + m_tile_padding) + m_tile_padding;
	int size_y = m_game->height() * (m_tile_size + m_tile_padding) + m_tile_padding;
	if (size_x < this->width()) {
		m_position.setX((this->width() - size_x) / 2);
	} else {
		if (m_position.x() > 0) m_position.setX(0);
		if (m_position.x() + size_x - this->width()  < 0) m_position.setX(this->width()  - size_x);
	}
	if (size_y < this->height()) {
		m_position.setY((this->height() - size_y) / 2);
	} else {
		if (m_position.y() > 0) m_position.setY(0);
		if (m_position.y() + size_y - this->height() < 0) m_position.setY(this->height() - size_y);
	}
}

void TilesWidget::relativeZoom(int x, int y, double factor)
{
	int new_tile_size = m_tile_size_f * factor;
	int new_tile_padding = (new_tile_size+12) / 16;
	int new_font_size = 0.6 * new_tile_size;
	
	QPoint old_abspos = QPoint(x - m_position.x(), y - m_position.y());
	QPoint tile = LUTileAtAbsolute(old_abspos.x(), old_abspos.y());
	QPoint old_anglep = QPoint(m_tile_padding + tile.x() * (m_tile_size + m_tile_padding)
	                          ,m_tile_padding + tile.y() * (m_tile_size + m_tile_padding));
	QPoint new_abspos = QPoint(new_tile_padding + tile.x() * (new_tile_size + new_tile_padding)
	                          ,new_tile_padding + tile.y() * (new_tile_size + new_tile_padding));
	
	m_position -= new_abspos - old_abspos;
	m_position -= QPoint(factor * (old_abspos.x()-old_anglep.x()), factor * (old_abspos.y()-old_anglep.y()));
	
	m_tile_size_f *= factor;
	m_tile_size = new_tile_size;
	m_tile_padding = new_tile_padding;
	m_font_size = new_font_size;
	clampCenterPosition();
}

void TilesWidget::receiveMouseEvent(QMouseEvent* e)
{
	m_mouse_position = e->pos() - m_position;
	m_hovered_tile = LUTileAtAbsolute(m_mouse_position.x() + m_tile_padding / 2, m_mouse_position.y() + m_tile_padding / 2);
	//if (m_pending_click_flags & CFMouse3) m_pressed_tile = m_hovered_tile;
}

void TilesWidget::fullRedraw()
{
	QPainter painter(this);
	TilesColorscheme colorscheme = m_settings->getColorscheme();
	painter.fillRect(0, 0, this->width(), this->height(), colorscheme.background);
	
	QPoint pmin = LUTile();
	QPoint pmax = RDTile();
	
	if (m_font_size > 0) {
		QFont font = painter.font();
		font.setPixelSize(m_font_size);
		painter.setFont(font);
	}
	for (int j = pmin.y(); j <= pmax.y(); ++j)
		for (int i = pmin.x(); i <= pmax.x(); ++i)
			drawTile(painter, i, j, colorscheme);
	emit updated();
}

void TilesWidget::mouseMoveEvent(QMouseEvent* e)
{
	receiveMouseEvent(e);
	this->update();
	e->accept();
}

void TilesWidget::mousePressEvent(QMouseEvent* e)
{
	receiveMouseEvent(e);
	m_pressed_tile = m_hovered_tile;
	
	m_pending_click_flags = 0;
	if (e->button() == Qt::LeftButton)   m_pending_click_flags |= CFMouse1;
	if (e->button() == Qt::RightButton)  m_pending_click_flags |= CFMouse2;
	if (e->button() == Qt::MiddleButton) m_pending_click_flags |= CFMouse3;
	if (e->button() == Qt::RightButton && m_settings->rightClickAsMiddle())
		m_pending_click_flags |= CFMouse3;
	Qt::KeyboardModifiers mods = QApplication::keyboardModifiers();
	if (mods.testFlag(Qt::ControlModifier)) m_pending_click_flags |= CFCtrl;
	if (mods.testFlag(Qt::ShiftModifier))   m_pending_click_flags |= CFShift;

	this->update();
	e->accept();
}

void TilesWidget::mouseReleaseEvent(QMouseEvent* e)
{
	receiveMouseEvent(e);
	//if (m_pressed_tile == m_hovered_tile )
		clicked(m_pressed_tile.x(), m_pressed_tile.y(), m_pending_click_flags);
	m_pressed_tile = QPoint(-2, -2);
	m_pending_click_flags = 0;
	this->update();
	e->accept();
}

void TilesWidget::leaveEvent(QEvent* event)
{
	(void)event;
	m_hovered_tile = QPoint(-2, -2);
	this->update();
	event->accept();
}

void TilesWidget::paintEvent(QPaintEvent* event)
{
	(void)event;
	fullRedraw();
	
	event->accept();
}

void TilesWidget::wheelEvent(QWheelEvent* event)
{
	QPoint numPixels = event->pixelDelta();
	QPoint numDegx8  = event->angleDelta();
	QPoint diff = numPixels.isNull() ? numDegx8/2 : numPixels;
	
	if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
		double new_sizef = m_tile_size_f * exp(0.002*(diff.x() + diff.y()));
		if (new_sizef < 4) new_sizef = 4;
		if (new_sizef > 200) new_sizef = 200;
		QPoint cp = event->pos();
		relativeZoom(cp.x(), cp.y(), new_sizef / m_tile_size_f);
	} else if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)) {
		m_position += QPoint(diff.x()+diff.y(), 0.);
		clampCenterPosition();
	} else {
		m_position += diff;
		clampCenterPosition();
	}
	
	event->accept();
}

void TilesWidget::resizeEvent(QResizeEvent* event)
{
	m_position += QPoint(
		 event->size().width()  / 2 - event->oldSize().width()  / 2
		,event->size().height() / 2 - event->oldSize().height() / 2);
	clampCenterPosition();
	event->accept();
}

TilesWidget::TilesWidget(Game* game, Settings* settings, QWidget *parent)
	: QWidget(parent)
	, m_position(0, 0)
	, m_mouse_position(-2, -2)
	, m_tile_size_f(24.)
	, m_hovered_tile(-2, -2)
	, m_pressed_tile(-2, -2)
	, m_pending_click_flags(0)
	, m_game(game)
	, m_settings(settings)
{
	this->setMouseTracking(true);
	m_tile_size = 24;
	m_tile_padding = 2;
	relativeZoom(0, 0, 1.0);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QSize TilesWidget::sizeHint() const
{
	return QSize(254, 254);
}

QSize TilesWidget::minimumSizeHint() const
{
	return QSize(86, 86);
}

void TilesWidget::setTileSize(int size)
{
	m_tile_size = size;
}

void TilesWidget::setGame(Game* game)
{
	Game* old_game = m_game;
	m_game = game;
	if (!old_game || old_game->width() != game->width() || old_game->height() != game->height()) {
		int size_x = game->width()  * (m_tile_size + m_tile_padding) + m_tile_padding;
		int size_y = game->height() * (m_tile_size + m_tile_padding) + m_tile_padding;
		m_position = QPoint((this->width() - size_x) / 2, (this->height() - size_y) / 2);
	}
	clampCenterPosition();
	this->update();
}
