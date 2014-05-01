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

bool Command::paramNumCheckOK(short min, short max)
{
	return ((tokenList.size() >= min) && (tokenList.size() <= max));
}

//
// FLUSHALL
// ARGS0
std::string Command::flushall()
{
	if (!paramNumCheckOK(MINARGFLUSHALL,MAXARGFLUSHALL)) throw CommandParameterError();
	std::string ret="";
	return ret;
}

//
// SETKV X Y 1200
// ARGS0 1 2    3
std::string Command::setkv()
{
	if (!paramNumCheckOK(MINARGSETKV,MAXARGSETKV)) throw CommandParameterError();
    std::string ret="";
	std::string expireTime="";

	if (tokenList.size() == MAXARGSETKV) {
		expireTime = " NOW() + INTERVAL '" + tokenList[MAXARGSETKV - 1] + " SECONDS' ";
	} else {
		expireTime = " NULL ";
	}

	ret = " UPDATE KVSTORE SET VALUE='" + tokenList[ MAXARGSETKV - 2 ] +"', ENTRY_DATE=NOW(), "
		  " EXPIRE_DATE=" + expireTime +" WHERE KEY_HASH=DIGEST('"+ tokenList[ MAXARGSETKV - 3 ] +"', 'MD5'); "
		  " INSERT INTO KVSTORE (KEY,KEY_HASH,VALUE,ENTRY_DATE,EXPIRE_DATE) SELECT "
		  " '"+ tokenList[ MAXARGSETKV - 3 ] +"', DIGEST('"+ tokenList[ MAXARGSETKV - 3 ] +"', 'MD5'), "
		  " '" + tokenList[ MAXARGSETKV - 2 ] + "',NOW(),"+ expireTime +
		  " WHERE NOT EXISTS (SELECT 1 FROM KVSTORE WHERE KEY_HASH=DIGEST('"+ tokenList[ MAXARGSETKV - 3 ] +"', 'MD5'));";
    return ret;
}

//
// GETKV X
// ARGS0 1
std::string Command::getkv()
{
	if (!paramNumCheckOK(MINARGGETKV,MAXARGGETKV)) throw CommandParameterError();
    std::string ret="";

    ret = " SELECT VALUE FROM KVSTORE WHERE "
    	  " (KEY_HASH=DIGEST('"+ tokenList[ MAXARGGETKV - 1 ] +"', 'MD5') AND (EXPIRE_DATE is NULL or EXPIRE_DATE > NOW()));";

    return ret;
}

//
// DELKV X
// ARGS0 1
std::string Command::delkv()
{
	if (!paramNumCheckOK(MINARGDELKV,MAXARGDELKV)) throw CommandParameterError();
    std::string ret="";

    ret = " DELETE FROM KVSTORE WHERE KEY_HASH=DIGEST('"+ tokenList[ MAXARGDELKV - 1 ] +"', 'MD5'); ";
    return ret;
}

//
// GETKEYS X%
//
std::string Command::getkeys()
{
	if (!paramNumCheckOK(MINARGGETKEYS,MAXARGGETKEYS)) throw CommandParameterError();
    std::string ret="";
    return ret;
}

//
// GETHOLDERS X%
//
std::string Command::getholders()
{
	if (!paramNumCheckOK(MINARGGETHOLDERS,MAXARGGETHOLDERS)) throw CommandParameterError();
    std::string ret="";
    return ret;
}

//
// CREATEHOLDER JSON XHOLDER
//
std::string Command::newholder()
{
	if (!paramNumCheckOK(MINARGNEWHOLDER,MAXARGNEWHOLDER)) throw CommandParameterError();
    std::string ret="";
    return ret;
}

//
// MODHOLDER XHOLDER ADDUNIQUEINDEX 'IDX_XHOLDER_NAME' 'NAME'
//
std::string Command::modholder()
{
	if (!paramNumCheckOK(MINARGMODHOLDER,MAXARGMODHOLDER)) throw CommandParameterError();
    std::string ret="";
    return ret;
}

//
// DELHOLDER XHOLDER
//
std::string Command::delholder()
{
	if (!paramNumCheckOK(MINARGDELHOLDER,MAXARGDELHOLDER)) throw CommandParameterError();
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

	if ( cmd.getPrefix() == PREFIXCOMMIT ) {
		ret = " COMMIT; ";
	} else if( cmd.getPrefix() == PREFIXFLUSHKV ) {
		ret = " TRUNCATE TABLE KVSTORE; ";
	} else if( cmd.getPrefix() == PREFIXFLUSHALL ) {
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
