/*
 * QSqlReadOnlyTableModel.h
 *
 *  Created on: Jul 22, 2015
 *      Author: awg
 */

#ifndef QSQLREADONLYTABLEMODEL_H_
#define QSQLREADONLYTABLEMODEL_H_

#include <QSqlTableModel>

class QSqlReadOnlyTableModel : public QSqlTableModel {
public:
	QSqlReadOnlyTableModel();
	virtual ~QSqlReadOnlyTableModel();
	const Qt::ItemFlags flags(const QModelIndex & index);
};

#endif /* QSQLREADONLYTABLEMODEL_H_ */
