#include "pgnosqlserver.h"
#include "config/configure.h"

int main () {
	std::map<std::string,std::string> config = getConfiguration();
	PgnosqlServer *server = new PgnosqlServer(config);
	try
	{
		server->init();
	} 
	catch (PGConnectionError err)
	{
		exit(-1);
	}
	server->run();
	delete server;
	return 0;
}
