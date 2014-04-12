#include "pgconnection.h"

int main () {

	//PGConnection *pgConn = new PGConnection("xxx","xxx");
	PGConnection pgConn("pgnosql","123456","localhost","pgnosqldb",5432);
	pgConn.connect();
    pgConn.disconnect();
	
	//delete pgConn;
	return 0;
}
