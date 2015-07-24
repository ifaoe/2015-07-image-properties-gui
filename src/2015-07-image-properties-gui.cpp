//============================================================================
// Name        : 2015-07-image-properties-gui.cpp
// Author      : Axel Wegener
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <QApplication>
#include "MainWindow.h"
#include "ConfigHandler.h"
#include "DatabaseHandler.h"

int main(int argc, char * argv[]) {
    QApplication::setDesktopSettingsAware(false);
    QApplication::setStyle("GTK+");
	QApplication app(argc,argv);
	ConfigHandler * cfg = new ConfigHandler(argc, argv);
	DatabaseHandler * db = new DatabaseHandler(cfg);
	MainWindow window(cfg,db);
	window.showMaximized();

	return app.exec();
}
