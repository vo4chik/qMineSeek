QT += widgets

CONFIG += c++11
#CONFIG += sanitizer sanitize_address

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	src/main.cpp \
	src/mainwindow.cpp \
	src/tile.cpp \
	src/game.cpp \
	src/tilestorage.cpp \
	src/settingsdialog.cpp \
	src/doublearray2d.cpp \
	src/minegenerator.cpp \
	src/tileswidget.cpp \
	src/doubleminarray2d.cpp \
	src/tilescolorscheme.cpp \
	src/kernels.cpp \
	src/settings.cpp

HEADERS += \
	src/mainwindow.h \
	src/tilestorage.h \
	src/tile.h \
	src/game.h \
	src/settingsdialog.h \
	src/doublearray2d.h \
	src/minegenerator.h \
	src/tileswidget.h \
	src/doubleminarray2d.h \
	src/tilescolorscheme.h \
	src/kernels.h \
	src/settings.h

DISTFILES +=

