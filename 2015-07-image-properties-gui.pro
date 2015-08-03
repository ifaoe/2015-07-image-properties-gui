QT += core gui sql

TARGET = daisi-img-prop
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

VPATH += ./src
SOURCES += 2015-07-image-properties-gui.cpp \
        MainWindow.cpp \
        ConfigHandler.cpp \
        DatabaseHandler.cpp \
        QtExtension/QSqlReadOnlyTableModel.cpp 

		
HEADERS  += MainWindow.h \
        ConfigHandler.h \
        DatabaseHandler.h \
        QtExtension/QSqlReadOnlyTableModel.h

VPATH += ./qt_ui
FORMS    += main_window.ui
			
INCLUDEPATH += /usr/include
DEPENDPATH += /usr/include

DEFINES += 

unix: LIBS += -L/usr/lib/ \
    -lboost_program_options
 
