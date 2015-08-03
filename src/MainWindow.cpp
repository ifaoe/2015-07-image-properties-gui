/*
 * MainWindow.cpp
 *
 *  Created on: Jul 22, 2015
 *      Author: awg
 */

#include "MainWindow.h"
#include <QInputDialog>

MainWindow::MainWindow(ConfigHandler * cfg, DatabaseHandler * db)
	: cfg(cfg), db(db), ui(new Ui::MainWindow) {
	// TODO Auto-generated constructor stub
	ui->setupUi(this);

	property_table = new QSqlReadOnlyTableModel(this, db->GetDatabase());
	ui->tableView_image_properties->setModel(property_table);

	connect(ui->actionMit_Server_verbinden, SIGNAL(triggered()),this, SLOT(HandleServerSelection()));
	connect(ui->actionProject_laden, SIGNAL(triggered()),this, SLOT(HandleSessionSelection()));

	connect(ui->buttonGroup_save_type,SIGNAL(buttonClicked(QAbstractButton*)),this, SLOT(HandleSaveData(QAbstractButton*)));

}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

void MainWindow::LoadSession() {
	InitFilters();
	ApplyFilters();
	UpdateProgress();
}

void MainWindow::InitFilters() {
	disconnect(ui->comboBox_cam, SIGNAL(currentIndexChanged(int)),this, SLOT(HandleCameraFilter(int)));
	disconnect(ui->comboBox_trc, SIGNAL(currentIndexChanged(int)),this, SLOT(HandleTracFilter(int)));
	disconnect(ui->lineEdit_image_filter, SIGNAL(editingFinished()),this, SLOT(HandleImageFilter()));

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
	SetTableQuery(filters.join(" AND "));
	ui->tableView_image_properties->resizeColumnsToContents();
	ui->tableView_image_properties->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::SetTableQuery(QString where) {
	property_table->setFilter(where);
}

void MainWindow::UpdateProgress() {
	ui->groupBox_glare->setTitle("Glare: " + db->GetPropertyProgress("glare_key"));
	ui->groupBox_seastate->setTitle("Seastate: " + db->GetPropertyProgress("seastate"));
	ui->groupBox_turbidity->setTitle("Turbidity: " + db->GetPropertyProgress("turbidity"));
	ui->groupBox_clarity->setTitle("Clarity: " + db->GetPropertyProgress("clarity"));
	ui->groupBox_ice->setTitle("Ice: " + db->GetPropertyProgress("ice"));
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

	property_table->setTable("image_properties");
	property_table->setEditStrategy(QSqlTableModel::OnManualSubmit);
	property_table->select();


	index_list.clear();
	index_list["glare_key"] = property_table->fieldIndex("glare_key");
	index_list["seastate"] = property_table->fieldIndex("seastate");
	index_list["turbidity"] =  property_table->fieldIndex("turbidity");
	index_list["clarity"] = property_table->fieldIndex("clarity");
	index_list["ice"] = property_table->fieldIndex("ice");
	index_list["remarks"] = property_table->fieldIndex("remarks");
	index_list["trc"] = property_table->fieldIndex("trc");
	index_list["cam"] = property_table->fieldIndex("cam");
	index_list["img"] = property_table->fieldIndex("img");

 	for (int i=0; i<property_table->columnCount(); i++) {
		if (!index_list.values().contains(i))
			ui->tableView_image_properties->hideColumn(i);
	}

 	ui->tableView_image_properties->horizontalHeader()->moveSection(index_list["trc"],0);
 	ui->tableView_image_properties->verticalHeader()->setResizeMode(QHeaderView::Fixed);

	property_table->setFilter("FALSE");
	property_table->set_order_by_clause("ORDER BY trc, cam, img");
}

void MainWindow::SetTableData(QString column_name, QVariant data) {
	int column = property_table->fieldIndex(column_name);
	QModelIndexList index_list = ui->tableView_image_properties->selectionModel()->selectedRows(column);
	QItemSelection old_selection = ui->tableView_image_properties->selectionModel()->selection();
	progress_dialog_ = new QProgressDialog(tr("Speichere aktuelle Auswahl..."), tr("Abbrechen"),0,old_selection.size(),this);
	progress_dialog_->setWindowModality(Qt::WindowModal);
	progress_dialog_->setMinimumDuration(0);
	progress_dialog_->show();
	QList<int> top_left_row, top_left_column;
	QList<int> bottom_right_row, bottom_right_column;
	for (int i=0; i<old_selection.size();i++) {
		progress_dialog_->setValue(i);
		top_left_row.append(old_selection[i].topLeft().row());
		top_left_column.append(old_selection[i].topLeft().column());
		bottom_right_row.append(old_selection[i].bottomRight().row());
		bottom_right_column.append(old_selection[i].bottomRight().column());
	}
	progress_dialog_->setLabelText(tr("Erstelle Datensatz"));
	progress_dialog_->setRange(0,index_list.size());
	progress_dialog_->setValue(0);
	for (int i=0; i<index_list.size(); i++) {
		progress_dialog_->setValue(i);
		if (progress_dialog_->wasCanceled())
			break;
		property_table->setData(index_list[i], data.toString());
	}
	progress_dialog_->setLabelText(tr("Schreibe Daten in Datenbank..."));
	database_progess_ = index_list.size();
	connect(property_table, SIGNAL(beforeUpdate(int, QSqlRecord&)), this, SLOT(UpdateDatabaseProgress()));
	property_table->submitAll();
	progress_dialog_->setLabelText(tr("Stelle Auswahl wieder her..."));
	progress_dialog_->setRange(0, top_left_row.size());
	QModelIndex top_left, bottom_right;
	QItemSelection selection ;
	for (int i=0; i<top_left_row.size(); i++) {
		progress_dialog_->setValue(i);
		if (progress_dialog_->wasCanceled())
			break;
		top_left = property_table->index(top_left_row[i],top_left_column[i],QModelIndex());
		bottom_right = property_table->index(bottom_right_row[i],bottom_right_column[i],QModelIndex());
		selection.select(top_left, bottom_right);
	}
	ui->tableView_image_properties->selectionModel()->select(selection,QItemSelectionModel::Select);
	UpdateProgress();
	delete progress_dialog_;
}

void MainWindow::HandleSessionSelection() {
	bool check;
	QString project = QInputDialog::getItem(this,tr("Flug auswählen"),tr("Flug:"),
			db->GetProjectList(),0,false,&check);
	if (check) {
		cfg->set_project(project);
		filter_map["session"] = QString("session='%1'").arg(project);
	} else {
		filter_map.remove("session");
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
		filter_map.remove("trc");
	else
		filter_map["trc"] = QString("trc='%1'").arg(ui->comboBox_trc->currentText());
	ApplyFilters();
}

void MainWindow::HandleCameraFilter(int index) {
	if (index == 0)
		filter_map.remove("cam");
	else
		filter_map["cam"] = QString("cam='%1'").arg(ui->comboBox_cam->currentText());
	ApplyFilters();
}

void MainWindow::HandleSaveData(QAbstractButton * button) {
	QString type = button->property("dbvalue").toString();
	if (type == "glare_key") {
		SetTableData(type, ui->comboBox_glare->itemData(ui->comboBox_glare->currentIndex()));
	} else if (type == "seastate") {
		SetTableData(type, ui->comboBox_seastate->itemData(ui->comboBox_seastate->currentIndex()));
	} else if (type == "turbidity") {
		SetTableData(type,ui->comboBox_turbidity->itemData(ui->comboBox_turbidity->currentIndex()));
	} else if (type == "clarity") {
		SetTableData(type, ui->comboBox_clarity->itemData(ui->comboBox_clarity->currentIndex()));
	} else if (type == "ice") {
		SetTableData(type, ui->comboBox_ice->itemData(ui->comboBox_ice->currentIndex()));
	} else if (type == "remarks") {
		SetTableData(type, ui->plainTextEdit_remarks->toPlainText());
	}
}

void MainWindow::UpdateDatabaseProgress() {
	progress_dialog_->setValue(progress_dialog_->maximum() - database_progess_);
	database_progess_--;
}
