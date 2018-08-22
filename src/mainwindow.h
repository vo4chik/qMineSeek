#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "game.h"
#include "tileswidget.h"
#include "minegenerator.h"
#include "settings.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
private:
	Game* m_game;
	TilesWidget* m_tileswidget;
	
	QLabel* m_mine_count_label_cnt;
	QLabel* m_finish_percentage_label;
	
	Settings m_settings;
	MineGenerator m_generator;
	
	Game* game() { return m_game; }
protected slots:
public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	
signals:
	
public slots:
	void newGame();
	void gameSettings();
	
	void updateStatusBar();
	
	void gameOver();
};

#endif // MAINWINDOW_H
