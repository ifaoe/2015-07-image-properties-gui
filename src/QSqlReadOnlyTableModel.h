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
	QSqlReadOnlyTableModel(QObject * parent, QSqlDatabase * db);
	virtual ~QSqlReadOnlyTableModel();
	void set_order_by_clause(QString clause) { order_by_clause_ = clause; }
//	const Qt::ItemFlags flags(const QModelIndex & index);
protected:
	QString orderByClause() const;
//	void QSqlTableModel::setQuery(const QSqlQuery & query);
private:
	QString order_by_clause_ = "";
};

#endif /* QSQLREADONLYTABLEMODEL_H_ */
