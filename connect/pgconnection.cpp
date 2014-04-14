#include "pgconnection.h"

	
//Constructors:
PGConnection::PGConnection(){

	username = "";
	password = "";
	hostname = "";
	dbname = "";
	port = 0;

	//configure LOGGING
	LOGGING_CONFIG

	logger = Logger::getLogger( "PGConnection");
	conn=NULL;

}

PGConnection::PGConnection(const std::string &username, const std::string &password,
		const std::string &hostname, const std::string &dbname, short port,
		bool fileLogging, bool consoleLogging)
		:LoggingOption(fileLogging, consoleLogging), username( username ),
		password( password ), hostname( hostname ), dbname( dbname ),
		port( port ) {

	//configure LOGGING
	LOGGING_CONFIG

	logger = Logger::getLogger( "PGConnection");
	conn=NULL;
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
		std::string conn_str = "user=" + username + " ";
					 conn_str += "host=" + hostname + " ";
					 conn_str += "password=" + password + " ";
					 conn_str += "dbname=" + dbname + " ";
					 conn_str += "port=" + std::to_string(port);
		conn = new pqxx::connection(conn_str);
	}
    catch (const std::exception &e)
	{
		LOG4CXX_FATAL(logger, "Can't connect to db "<<e.what());
		return false;
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

//Destructor:
PGConnection::~PGConnection() {
	if ( conn != NULL) {
		delete conn;
	}
	LOG4CXX_INFO(logger, "Finalizing PGConnection");
}

