#include "mainwindow.h"
#include "settingsdialog.h"
#include "kernels.h"

#include <QAction>
#include <QActionGroup>
#include <QLabel>
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_game(nullptr)
	, m_tileswidget(nullptr)
{
	m_settings.load();
	m_settings.applyToGenerator(&m_generator);
	
	auto game_menu = menuBar()->addMenu(QString("&Game"));
		auto new_game_action = new QAction(QString("New Game"), this);
		new_game_action->setShortcuts(QKeySequence::New);
		new_game_action->setStatusTip(QString("Start a new game"));
		QObject::connect(new_game_action, &QAction::triggered, this, &MainWindow::newGame);
		game_menu->addAction(new_game_action);
		
		auto settings_action = new QAction(QString("Settings"), this);
		settings_action->setStatusTip(QString("Modify field settings"));
		QObject::connect(settings_action, &QAction::triggered, this, &MainWindow::gameSettings);
		game_menu->addAction(settings_action);
		
		auto quit_action = new QAction(QString("Quit"), this);
		quit_action->setShortcuts(QKeySequence::Quit);
		quit_action->setStatusTip(QString("Leave this"));
		QObject::connect(quit_action, &QAction::triggered, this, &MainWindow::close);
		game_menu->addAction(quit_action);
	
	auto help_menu = menuBar()->addMenu(QString("&Help"));
		auto ctrls_action = new QAction(QString("Controls"), this);
		ctrls_action->setStatusTip(QString("Show controls help"));
		connect(ctrls_action, &QAction::triggered, [](){
			QMessageBox help;
			help.setWindowTitle("Minesweeper Controls");
			help.setText(QString(
				"<b>Left Click</b>: open tile<br>"
				"<b>Right Click</b>: place/remove flag<br>"
				"<b>Middle Click</b>: act nearby (open tiles, place flags)<br>"
				"<b>Ctrl + Right Click</b>: place question mark<br>"
				"<b>Mouse Wheel</b>: scroll vertically<br>"
				"<b>Shift + Mouse Wheel</b>: scroll horizontally<br>"
				"<b>Ctrl + Mouse Wheel</b>: zoom"));
			help.exec();
		});
		help_menu->addAction(ctrls_action);
		
	
	auto status_bar = this->statusBar();
		m_mine_count_label_cnt = new QLabel(this);
		status_bar->addWidget(m_mine_count_label_cnt);
		m_finish_percentage_label = new QLabel(this);
		status_bar->addWidget(m_finish_percentage_label);
	
	QSettings s;
	if (s.contains(QString("mainwindow/geometry"))) {
		this->restoreGeometry(s.value(QString("mainwindow/geometry")).toByteArray());
		this->restoreState   (s.value(QString("mainwindow/state"))   .toByteArray());
	}
	
	newGame();
}

MainWindow::~MainWindow()
{
	QSettings s;
	s.setValue(QString("mainwindow/geometry"), saveGeometry());
	s.setValue(QString("mainwindow/state")   , saveState());
	m_settings.save();
}

void MainWindow::newGame()
{
	setUpdatesEnabled(false);
	
	Game* old_game = m_game;
	m_game = new Game(m_generator, this);
	if (m_tileswidget)
		m_tileswidget->setGame(m_game);
	else {
		m_tileswidget = new TilesWidget(m_game, &m_settings);
		setCentralWidget(m_tileswidget);
	}
	setUpdatesEnabled(true);
	if (old_game) delete old_game;
	
	QObject::connect(m_tileswidget, &TilesWidget::updated, this, &MainWindow::updateStatusBar);
	QObject::connect(m_game, &Game::gameOver, this, &MainWindow::gameOver);
	updateStatusBar();
}

void MainWindow::gameSettings()
{
	auto diag = new SettingsDialog(&m_settings);
	if (diag->exec() == QDialog::Accepted) {
		m_settings.applyToGenerator(&m_generator);
		m_tileswidget->update();
		if (m_game->state() == Game::NotStarted) newGame();
	}
	delete diag;
}

void MainWindow::updateStatusBar()
{
	m_mine_count_label_cnt->setText(
		QString("Mines: %1/%2").arg(m_game->seeminglyUnfoundMineCount()).arg(m_game->mineCount()));
	m_finish_percentage_label->setText(
		QString("Progress: %1").arg(QString::asprintf("%.1f%%",
			100. * m_game->openedCount() / (m_game->width() * m_game->height() - m_game->mineCount()))));
}

void MainWindow::gameOver()
{
	QMessageBox* msg = new QMessageBox(this);
	msg->setText(m_game->isWon() ? QString("You won") : QString("You lost"));
	msg->exec();
	delete msg;
}

