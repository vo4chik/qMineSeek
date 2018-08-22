#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setOrganizationName(QString("vo4chik"));
	a.setApplicationName(QString("qMineSeek"));
	a.setApplicationVersion(QString("0.1.0"));
	MainWindow mainwindow;
	mainwindow.show();
	
	return a.exec();
}
