/*
 * QSqlReadOnlyTableModel.cpp
 *
 *  Created on: Jul 22, 2015
 *      Author: awg
 */

#include "QSqlReadOnlyTableModel.h"

QSqlReadOnlyTableModel::QSqlReadOnlyTableModel(QObject * parent, QSqlDatabase * db) {
	QSqlTableModel(parent,*db);
}

QSqlReadOnlyTableModel::~QSqlReadOnlyTableModel() {
	// TODO Auto-generated destructor stub
}
//
//const Qt::ItemFlags QSqlReadOnlyTableModel::flags(const QModelIndex & index) {
//	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
//}

//void QSqlTableModel::setQuery(const QSqlQuery & query) {
//
//}

QString QSqlReadOnlyTableModel::orderByClause() const{
	return order_by_clause_;
}
