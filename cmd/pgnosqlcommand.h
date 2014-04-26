#ifndef __PGNOSQL_COMMAND__
#define __PGNOSQL_COMMAND__

#include <boost/algorithm/string.hpp>
#include "common.h"
#include "commands.h"


class Command
{
private:
	std::string cmdStr;
	std::list<std::string> tokenList;
public:
	Command(std::string cmdString);
	bool tokenize();
	std::list<std::string> getTokenList(){ return tokenList; }
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
