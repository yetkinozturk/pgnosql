#ifndef _PGNOSQLSERVER_H
#define _PGNOSQLSERVER_H

#include "common/common.h"
#include "connect/pgconnection.h"
#include "logging/loggingoption.h"

class PgnosqlServer
{

private:
	LoggingOption logOption;
	LoggerPtr logger;
	PGConnection *pgConn;
	std::string username;
	std::string password;
	unsigned int connLimit;
	short port;
public:
	PgnosqlServer();
	PgnosqlServer(std::map<std::string,std::string> config);
	int setSocket(int sfd);
	int bindSocket();
	int init();
	int run ();
	~PgnosqlServer();
};

#endif
