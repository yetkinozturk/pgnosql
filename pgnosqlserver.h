#ifndef _PGNOSQLSERVER_H
#define _PGNOSQLSERVER_H

#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <cerrno>
#include "common.h"
#include "pgconnection.h"
#include "loggingoption.h"

class PgnosqlServer : LoggingOption
{

private:
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
	int run ();
	~PgnosqlServer();
};

#endif
