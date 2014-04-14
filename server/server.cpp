#include "pgnosqlserver.h"
#include "config/configure.h"

int main () {
	std::map<std::string,std::string> config = getConfiguration();
	PgnosqlServer *server = new PgnosqlServer(config);
	server->run();
	delete server;
	return 0;
}
