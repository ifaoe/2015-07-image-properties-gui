/*
 * MainWindow.cpp
 *
 *  Created on: Jul 22, 2015
 *      Author: awg
 */

#include "MainWindow.h"
#include <QInputDialog>
#include <QDebug>
#include <iostream>

MainWindow::MainWindow(ConfigHandler * cfg, DatabaseHandler * db)
	: cfg(cfg), db(db), ui(new Ui::MainWindow) {
	// TODO Auto-generated constructor stub
	ui->setupUi(this);

	property_table = new QSqlQueryModel;
	ui->tableView_image_properties->setModel(property_table);
	ui->tableView_image_properties->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	connect(ui->actionMit_Server_verbinden, SIGNAL(triggered()),this, SLOT(HandleServerSelection()));
	connect(ui->actionProject_laden, SIGNAL(triggered()),this, SLOT(HandleSessionSelection()));
}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

void MainWindow::LoadSession() {
	property_table->clear();
	InitFilters();
	ApplyFilters();
}

void MainWindow::InitFilters() {
	ui->comboBox_cam->clear();
	ui->comboBox_cam->addItems(db->GetProjectCams());
	ui->comboBox_trc->clear();
	ui->comboBox_trc->addItems(db->GetProjectTracs());

	connect(ui->comboBox_cam, SIGNAL(currentIndexChanged(int)),this, SLOT(HandleCameraFilter(int)));
	connect(ui->comboBox_trc, SIGNAL(currentIndexChanged(int)),this, SLOT(HandleTracFilter(int)));
	connect(ui->lineEdit_image_filter, SIGNAL(editingFinished()),this, SLOT(HandleImageFilter()));
}

void MainWindow::ApplyFilters() {
	ui->tableView_image_properties->clearSelection();
	QStringList filters = filter_map.values();
	filters.prepend("TRUE");
	SetTableQuery(filters.join(" AND "));
}

void MainWindow::SetTableQuery(QString where = "TRUE") {
	QString query = "SELECT trc as Transekt, cam as Kamera, img as Bildnummer, "
				"glare_key as Glare, seastate, turbidity, clarity, ice FROM image_properties WHERE session='%1' "
				"AND %2 ORDER BY trc, cam, img";
	qDebug() << query;
	std::cout << query.arg(cfg->project()).arg(where).toStdString() << std::endl;
	property_table->setQuery(query.arg(cfg->project()).arg(where));
}

void MainWindow::HandleServerSelection() {
	bool check;
	QString server_location = QInputDialog::getItem(this,tr("Mit Server verbinden"),tr("Server:"),
			cfg->database_list(),0,false,&check);
	if (check) {
		cfg->parse_database_info(server_location);
		ui->actionProject_laden->setEnabled(true);
	} else {
		ui->actionProject_laden->setEnabled(false);
		return;
	}
	db->OpenDatabase();

	db->GetStuk4Codes("CLARITY",ui->comboBox_clarity);
	db->GetStuk4Codes("GLARE",ui->comboBox_glare);
	db->GetStuk4Codes("SEASTATE",ui->comboBox_seastate);
	db->GetStuk4Codes("ICE",ui->comboBox_ice);
	db->GetStuk4Codes("TURBIDITY",ui->comboBox_turbidity);
}

void MainWindow::HandleSessionSelection() {
	bool check;
	QString project = QInputDialog::getItem(this,tr("Flug auswählen"),tr("Flug:"),
			db->GetProjectList(),0,false,&check);
	if (check) {
		cfg->set_project(project);
	} else {
		return;
	}
	LoadSession();
}

void MainWindow::HandleImageFilter() {
	QString text = ui->lineEdit_image_filter->text();
	if (text.isEmpty())
		text = "%";
	QString filter_text;
	if(text.contains("HD",Qt::CaseInsensitive)) {
		filter_text = text;
	} else {
		filter_text = "HD" + text;
	}
	filter_map["img"] = QString("img like '%1'").arg(filter_text);
	ApplyFilters();
}

void MainWindow::HandleTracFilter(int index) {
	if (index == 0)
		filter_map["trc"] = "TRUE";
	else
		filter_map["trc"] = QString("trc='%1'").arg(ui->comboBox_trc->currentText());
	ApplyFilters();
}

void MainWindow::HandleCameraFilter(int index) {
	if (index == 0)
		filter_map["cam"] = "TRUE";
	else
		filter_map["cam"] = QString("cam='%1'").arg(ui->comboBox_cam->currentText());
	ApplyFilters();
}


