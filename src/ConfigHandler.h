/*
 * ConfigHandler.h
 *
 *  Created on: Jul 22, 2015
 *      Author: awg
 */

#ifndef CONFIGHANDLER_H_
#define CONFIGHANDLER_H_

#include <QFile>
#include <QStringList>
#include <boost/property_tree/ptree.hpp>

struct DatabaseInfo {
	QString name = "";
	QString host = "127.0.0.1";
	int port = 5432;
	QString user = "daisi";
	QString db = "daisi";
	QString password = "18ifaoe184";
};

class ConfigHandler {
public:
	ConfigHandler(int argc, char * argv[]);
	virtual ~ConfigHandler();
	bool parse_database_info(QString db_name);
	QString db_location() {return db_info_->name; }
	QString db_host() {return db_info_->host;}
	int db_port() {return db_info_->port;}
	QString db_name() {return db_info_->db;}
	QString db_password() {return db_info_->password;}
	QString db_user() {return  db_info_->user;}
	void set_project(QString project) {project_ = project;}
	QString project() {return project_;}
	QStringList database_list() {return database_list_;}
private:
	QFile * cfg_file_ = 0;
	boost::property_tree::ptree cfg_tree;
	QStringList database_list_;
	DatabaseInfo * db_info_=0;
	QString project_;
};



#endif /* CONFIGHANDLER_H_ */
