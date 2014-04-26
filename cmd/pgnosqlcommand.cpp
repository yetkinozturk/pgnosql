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
	return true;
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
	boost::to_upper(token.front());

	if( token.front() == PREFIXFLUSHALL ) {
		ret = cmd.flushall();
	} else if ( token.front() == PREFIXSETKV ) {
		ret = cmd.setkv();
	} else if ( token.front() == PREFIXGETKV ) {
		ret = cmd.getkv();
	} else if ( token.front() == PREFIXDELKV ) {
        ret = cmd.delkv();
    } else if ( token.front() == PREFIXGETKEYS ) {
        ret = cmd.getkeys();
    } else if ( token.front() == PREFIXGETHOLDERS) {
		ret = cmd.getholders();
	} else if ( token.front() == PREFIXNEWHOLDER) {
        ret = cmd.newholder();
    } else if ( token.front() == PREFIXMODHOLDER) {
        ret = cmd.modholder();
    } else if ( token.front() == PREFIXDELHOLDER) {
        ret = cmd.delholder();
    } else {
		throw UnknownCommandError();
	}
	
	return ret;

}
