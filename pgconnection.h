#ifndef __PGCONNECTION__   
#define __PGCONNECTION__

#include "common.h"

class PGConnection
{

private:
	std::string username;
	std::string password;
	std::string hostname;
	std::string dbname;
	short port;
	pqxx::connection *conn;
	LoggerPtr logger;
public:
	
	//Constructors:
	PGConnection();

	PGConnection(const std::string &username, const std::string &password, 
				 const std::string &hostname, const std::string &dbname, short port);

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
