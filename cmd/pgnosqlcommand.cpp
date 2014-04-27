#include "pgnosqlcommand.h"

Command::Command(std::string str)
{
	cmdStr=str;
}

bool Command::tokenize()
{
	std::string token1,token2;

	if ( cmdStr.length() < MINCMDLEN || cmdStr.length() > MAXCMDLEN )
	{
		return false;
	}

    for( size_t i=0; i<cmdStr.length(); i++){

        char c = cmdStr[i];
        if( c == ' ' )
        {
            if(token1.length() > 0)
            {
                tokenList.push_back(token1);
                token1 = "";
            }
        }
        else if(c == '\"' )
        {
            i++;
            token2="";
            while( i<cmdStr.length() && cmdStr[i] != '\"' ){ token2.append(1,cmdStr[i]); i++; }
            if (token2.length() > 0)
            {
                tokenList.push_back(token2);
            }
        }
        else if(c == '\'')
        {
            i++;
            token2="";
            while( i<cmdStr.length() && cmdStr[i] != '\'' ){ token2.append(1,cmdStr[i]); i++; }
            if (token2.length() > 0)
            {
                tokenList.push_back(token2);
            }
        }
        else
        {
            token1.append(1,c);
        }
    }
    if(token1.length() > 0)
    {
        tokenList.push_back(token1);
        token1="";
    }
	if (tokenList.empty())
	{
		return false;
	}

	prefix = boost::to_upper_copy(tokenList.front());

	return true;
}

std::string Command::flushall()
{
	std::string ret="";
	return ret;
}

std::string Command::setkv()
{
    std::string ret="";
    return ret;
}

std::string Command::getkv()
{
    std::string ret="";
    return ret;
}

std::string Command::delkv()
{
    std::string ret="";
    return ret;
}

std::string Command::getkeys()
{
    std::string ret="";
    return ret;
}

std::string Command::getholders()
{
    std::string ret="";
    return ret;
}

std::string Command::newholder()
{
    std::string ret="";
    return ret;
}

std::string Command::modholder()
{
    std::string ret="";
    return ret;
}

std::string Command::delholder()
{
    std::string ret="";
    return ret;
}

PgnosqlCommand::PgnosqlCommand(std::string str)
{
	cmdStr = str;
	boost::trim(cmdStr);
	cmd = Command(cmdStr);
}

std::string PgnosqlCommand::translate()
{
	if (!cmd.tokenize())
	{
		throw CommandParseError();
	}

	std::string ret="";

	if( cmd.getPrefix() == PREFIXFLUSHALL ) {
		ret = cmd.flushall();
	} else if ( cmd.getPrefix() == PREFIXSETKV ) {
		ret = cmd.setkv();
	} else if ( cmd.getPrefix() == PREFIXGETKV ) {
		ret = cmd.getkv();
	} else if ( cmd.getPrefix() == PREFIXDELKV ) {
        ret = cmd.delkv();
    } else if ( cmd.getPrefix() == PREFIXGETKEYS ) {
        ret = cmd.getkeys();
    } else if ( cmd.getPrefix() == PREFIXGETHOLDERS) {
		ret = cmd.getholders();
	} else if ( cmd.getPrefix() == PREFIXNEWHOLDER) {
        ret = cmd.newholder();
    } else if ( cmd.getPrefix() == PREFIXMODHOLDER) {
        ret = cmd.modholder();
    } else if ( cmd.getPrefix() == PREFIXDELHOLDER) {
        ret = cmd.delholder();
    } else {
		throw UnknownCommandError();
	}
	
	return ret;
}