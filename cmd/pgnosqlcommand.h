#ifndef __PGNOSQL_COMMAND__
#define __PGNOSQL_COMMAND__

#include <boost/algorithm/string.hpp>
#include "common/common.h"
#include "commands.h"


class Command
{
private:
	std::string cmdStr;
	std::string prefix;
	std::list<std::string> tokenList;
public:
	Command(){}
	Command(std::string cmdString);
	bool tokenize();
	std::string getPrefix(){ return prefix; }
	std::string flushall();
	std::string setkv();
	std::string getkv();
	std::string delkv();
	std::string getkeys();
	std::string getholders();
	std::string newholder();
	std::string modholder();
	std::string delholder();
	~Command(){}
};

class PgnosqlCommand
{
private:
	std::string cmdStr;
	Command cmd;
public:
	PgnosqlCommand(std::string cmdStr);
	std::string translate();
	~PgnosqlCommand(){}
};
#endif
