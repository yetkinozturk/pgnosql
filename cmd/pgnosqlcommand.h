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
	std::vector<std::string> tokenList;
public:
	Command(){}
	Command(std::string cmdString);
	void removeExtraSpaces(std::string &str);
	void makeProperJsonQuotes(std::string &str);
	bool tokenize();
	bool paramNumCheckOK(short min, short max);
	bool isFirstLevelCondition(std::string expr);
	std::string getPrefix(){ return prefix; }
	std::string flushall();
	std::string setkv();
	std::string getkv();
	std::string delkv();
	std::string getkeys();
	std::string getholders();
	std::string newholder();
	std::string existholder();
	std::string modholder();
	std::string opholder();
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
