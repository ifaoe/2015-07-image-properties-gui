/*
 * ConfigHandler.cpp
 *
 *  Created on: Jul 22, 2015
 *      Author: awg
 */

#include <iostream>
#include <QDebug>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/program_options.hpp>
#include "ConfigHandler.h"

namespace po = boost::program_options;
namespace pt = boost::property_tree;
using namespace std;

ConfigHandler::ConfigHandler(int argc, char * argv[]) {
	db_info_ = new DatabaseInfo;

    po::options_description desc("Options");
    desc.add_options()
            ("help,h", "Show this help message.")
            ("config,c", po::value<string>(), "Path to config-file.");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        exit(0);
    }
    if (vm.count("config")) {
        string tmp = vm["config"].as<string>();
        cfg_file_ = new QFile(tmp.data());
    } else {
        cfg_file_ = new QFile("/usr/local/ifaoe/settings/daisi/property-editor/main.cfg");
    }
    if (cfg_file_->exists()) {
        qDebug() << "Using config-file: " << cfg_file_->fileName();
    } else {
        qFatal( "Fatal: Config file %s does not exist or is not readable. Aborting.", cfg_file_->fileName().toStdString().c_str() );
    }
    boost::property_tree::ini_parser::read_ini(cfg_file_->fileName().toStdString(), cfg_tree);
	database_list_ = QString().fromStdString( cfg_tree.get<std::string>("main.dblist")).split(",");
}

ConfigHandler::~ConfigHandler() {
	delete db_info_;
	delete cfg_file_;
}

bool ConfigHandler::parse_database_info(QString db_name) {
	delete db_info_;
	db_info_ = new DatabaseInfo;
	if (cfg_tree.count(db_name.toStdString()) == 0) {
		qDebug() << "Warning in ConfigHandler::parse_database_info: Server " << db_name << " does not exist.";
		return false;
	}
	db_info_->name = db_name;
    db_info_->host = QString::fromStdString( cfg_tree.get<std::string>(db_name.toStdString() + ".host") );
    db_info_->db = QString::fromStdString( cfg_tree.get<std::string>(db_name.toStdString() + ".name") );
    db_info_->user = QString::fromStdString( cfg_tree.get<std::string>(db_name.toStdString() + ".user") );
    db_info_->password = QString::fromStdString( cfg_tree.get<std::string>(db_name.toStdString() + ".pass") );
    db_info_->port =  cfg_tree.get<int>(db_name.toStdString() + ".port");
    return true;
}
