#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QGroupBox>
#include <QTabWidget>
#include "minegenerator.h"
#include "settings.h"

class SettingsDialog : public QDialog
{
	Q_OBJECT
	
	Settings* m_settings;
	Settings* m_out_settings;
	
	QGroupBox* createSizeGroup();
	QGroupBox* createMinegenGroup();
	QWidget* createMineGenerationPage();
	QWidget* createGameplayPage();
	QTabWidget* createTabWidget();
public:
	SettingsDialog(Settings* settings, QWidget* parent = nullptr);
	~SettingsDialog();
};

#endif // SETTINGSDIALOG_H
