/*
 * QSqlReadOnlyTableModel.cpp
 *
 *  Created on: Jul 22, 2015
 *      Author: awg
 */

#include "QSqlReadOnlyTableModel.h"

QSqlReadOnlyTableModel::QSqlReadOnlyTableModel() {
	// TODO Auto-generated constructor stub
}

QSqlReadOnlyTableModel::~QSqlReadOnlyTableModel() {
	// TODO Auto-generated destructor stub
}

const Qt::ItemFlags QSqlReadOnlyTableModel::flags(const QModelIndex & index) {
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
