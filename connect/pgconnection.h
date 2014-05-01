#ifndef __PGCONNECTION__   
#define __PGCONNECTION__

#include "common/common.h"
#include "logging/loggingoption.h"

class PGConnection
{

private:
	LoggingOption logOption;
	pqxx::connection *conn;
	LoggerPtr logger;
	std::map<std::string,std::string> config;
public:
	
	//Constructors:
	PGConnection();

	PGConnection(std::map<std::string,std::string> config);

	//Check Connection Status
	bool isConnected();

	//Connect to db
	bool connect();

	//Disconnect from db
	bool disconnect();

	//Get connection handler
	pqxx::connection* getConnection();

	//Destructor:
	~PGConnection();

};

#endif 
