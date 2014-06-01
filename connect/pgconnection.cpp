#include "pgconnection.h"

	
//Constructors:
PGConnection::PGConnection(){

	logOption.fileLogging = false;
	logOption.consoleLogging = true;

	//configure LOGGING
	LOGGING_CONFIG

	logger = Logger::getLogger( "PGConnection");
	conn = nullptr;

}

PGConnection::PGConnection(std::map<std::string,std::string> config)
		:config( config ) 
{

    logOption.fileLogging = (std::stoi(config["filelog"]) == 1);
	logOption.consoleLogging = (std::stoi(config["consolelog"]) == 1);

	//configure LOGGING
	LOGGING_CONFIG

	logger = Logger::getLogger( "PGConnection");
	conn = nullptr;
}

//Check Connection Status
bool PGConnection::isConnected() {
    try
    {
		return (conn != NULL) && (conn->is_open());
    }
    catch (const std::exception &e)
    {
		LOG4CXX_FATAL(logger, e.what());
        return false;
    }
}

//Establish connection
bool PGConnection::connect() {
	LOG4CXX_INFO(logger, "PGConnection -> connect");
	try
	{
		std::string conn_str = "user=" + config["pg_username"] + " ";
					 conn_str += "host=" + config["pg_hostname"] + " ";
					 conn_str += "password=" + config["pg_password"] + " ";
					 conn_str += "dbname=" + config["pg_dbname"] + " ";
					 conn_str += "port=" + config["pg_port"];
		conn = new pqxx::connection(conn_str);
	}
    catch (const std::exception &e)
	{
		LOG4CXX_FATAL(logger, "Can't connect to db "<<e.what());
		throw PGConnectionError();
	}
	return true;
}

//Disconnect from db
bool PGConnection::disconnect() {
	LOG4CXX_INFO(logger, "PGConnection -> disconnect");
	try 
	{
		if ( isConnected()) {
			conn->disconnect();
			return true;
		}
		return false;
	}
	catch (const std::exception &e)
	{
		LOG4CXX_FATAL(logger, "PGConnection::disconnect() failed! "<<e.what());
		return false;
	}
}

pqxx::connection* PGConnection::getConnection() { 
	return conn;
}

std::string PGConnection::getConfig(std::string key) {
	auto it = config.find(key);
	if (it != std::end(config)){
		return config[key];
	} else {
		return "UNKNOWN";
	}
}

//Destructor:
PGConnection::~PGConnection() {
	if ( conn != nullptr) {
		delete conn;
		conn = nullptr;
	}
	LOG4CXX_INFO(logger, "Finalizing PGConnection");
}

