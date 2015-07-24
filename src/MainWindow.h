/*
 * MainWindow.h
 *
 *  Created on: Jul 22, 2015
 *      Author: awg
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "ui_main_window.h"
#include <QMainWindow>
#include "ConfigHandler.h"
#include "DatabaseHandler.h"
#include "QSqlReadOnlyTableModel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(ConfigHandler * cfg, DatabaseHandler * db);
	virtual ~MainWindow();
private:
	ConfigHandler * cfg = 0;
	DatabaseHandler * db = 0;
	Ui::MainWindow * ui = 0;
	QMap<QString, QString> filter_map;
	QSqlReadOnlyTableModel * property_table;
	QMap<QString,int> index_list;
	void LoadSession();
	void InitFilters();
	void ApplyFilters();
	void SetTableQuery(QString where);
	void SetTableData(QString column_name, QVariant data);
	void UpdateProgress();
private slots:
	void HandleServerSelection();
	void HandleSessionSelection();
	void HandleImageFilter();
	void HandleTracFilter(int index);
	void HandleCameraFilter(int index);
	void HandleSaveGlareData();
	void HandleSaveSeastateData();
	void HandleSaveTurbidityData();
	void HandleSaveIceData();
	void HandleSaveClarityData();
};

#endif /* MAINWINDOW_H_ */
