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
	std::string ret;
	return ret;
}

//
// SETKV X Y 1200
// ARGS0 1 2    3
std::string Command::setkv()
{
	if (!paramNumCheckOK(MINARGSETKV,MAXARGSETKV)) throw CommandParameterError();
    std::string ret;
	std::string expireTime;

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
    std::string ret;

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
    std::string ret;

    ret = " DELETE FROM KVSTORE WHERE KEY_HASH=DIGEST('"+ tokenList[ MAXARGDELKV - 1 ] +"', 'MD5'); ";
    return ret;
}

//
// GETKEYS X%
// ARGS  0  1
std::string Command::getkeys()
{
	if (!paramNumCheckOK(MINARGGETKEYS,MAXARGGETKEYS)) throw CommandParameterError();
    std::string ret;
    ret = " SELECT KEY FROM KVSTORE WHERE (KEY LIKE '"+ tokenList[ MAXARGGETKEYS - 1 ] +"' AND"
    	  " (EXPIRE_DATE is NULL or EXPIRE_DATE > NOW()));";
    return ret;
}

//
// GETHOLDERS X%
// ARGS     0  1
std::string Command::getholders()
{
	if (!paramNumCheckOK(MINARGGETHOLDERS,MAXARGGETHOLDERS)) throw CommandParameterError();
    std::string ret;
    ret = " SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME LIKE '"+ tokenList[ MAXARGGETHOLDERS - 1 ] +"'; ";
    return ret;
}

//
// EXISTHOLDER X
// ARGS      0 1
std::string Command::existholder()
{
	if (!paramNumCheckOK(MINARGEXISTHOLDER,MAXARGEXISTHOLDER)) throw CommandParameterError();
    std::string ret;
    ret = " SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME='"+ tokenList[ MAXARGEXISTHOLDER - 1 ] +"'; ";
    return ret;
}

//
// CREATEHOLDER JSON X
// ARGS       0    1 2
std::string Command::newholder()
{
	if (!paramNumCheckOK(MINARGNEWHOLDER,MAXARGNEWHOLDER)) throw CommandParameterError();
    std::string ret;
    ret = " CREATE TABLE "+ tokenList[ MAXARGNEWHOLDER - 1 ] +" ( ID SERIAL PRIMARY KEY, DATA JSON ); "
    	  " INSERT INTO USER_TABLES (TABLE_NAME,TABLE_TYPE) VALUES ('"+ tokenList[ MAXARGNEWHOLDER - 1 ] +"','HOLDER');";
    return ret;
}

//
// MODHOLDER X ADDUNIQUEINDEX 'IDX_X_NAME' 'author'->>'first_name'
// MODHOLDER X 	     ADDINDEX 'IDX_X_NAME' 'author'->>'first_name'
// MODHOLDER X    DELETEINDEX 'IDX_X_NAME'
// ARGS    0 1              2            3                       4
std::string Command::modholder()
{
	if (!paramNumCheckOK(MINARGMODHOLDER,MAXARGMODHOLDER)) throw CommandParameterError();
    std::string ret;
    std::string indexType;
    boost::to_upper(tokenList[ MAXARGMODHOLDER - 3 ]);

    if (tokenList[ MAXARGMODHOLDER - 3 ].substr(0,3) == "ADD" ) {
    	if (tokenList[ MAXARGMODHOLDER - 3 ] == "ADDUNIQUEINDEX") {
    		indexType = " UNIQUE INDEX ";
    	} else {
    		indexType = " INDEX ";
    	}
    	ret = " CREATE " + indexType + tokenList[ MAXARGMODHOLDER - 2 ] + " ON " + tokenList[ MAXARGMODHOLDER - 4 ] +
              " ((data->" + tokenList[ MAXARGMODHOLDER - 1 ]+")); ";
    } else {
    	ret = " DROP INDEX IF EXISTS " + tokenList[ MAXARGMODHOLDER - 2 ] + "; ";
    }
    return ret;
}

//
// OPHOLDER
//
std::string Command::opholder()
{
	if (!paramNumCheckOK(MINARGMODHOLDER,MAXARGMODHOLDER)) throw CommandParameterError();
	std::string ret;
	return ret;
}

//
// DELHOLDER XHOLDER
// ARGS    0       1
std::string Command::delholder()
{
	if (!paramNumCheckOK(MINARGDELHOLDER,MAXARGDELHOLDER)) throw CommandParameterError();
    std::string ret;

    ret = " DROP TABLE IF EXISTS " + tokenList[ MAXARGDELHOLDER - 1 ] +"; " +
    	  " DELETE FROM USER_TABLES WHERE TABLE_NAME='"+ tokenList[ MAXARGDELHOLDER - 1 ] +"'; ";
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
	std::string prefix = cmd.getPrefix();

	if ( prefix == PREFIXCOMMIT ) {
		ret = " COMMIT; ";
	} else if( prefix == PREFIXFLUSHKV ) {
		ret = " TRUNCATE TABLE KVSTORE; ";
	} else if( prefix == PREFIXFLUSHALL ) {
		ret = cmd.flushall();
	} else if ( prefix == PREFIXSETKV ) {
		ret = cmd.setkv();
	} else if ( prefix == PREFIXGETKV ) {
		ret = cmd.getkv();
	} else if ( prefix == PREFIXDELKV ) {
        ret = cmd.delkv();
    } else if ( prefix == PREFIXGETKEYS ) {
        ret = cmd.getkeys();
    } else if ( prefix == PREFIXGETHOLDERS) {
		ret = cmd.getholders();
	} else if ( prefix == PREFIXNEWHOLDER) {
        ret = cmd.newholder();
    } else if ( prefix == PREFIXEXISTHOLDER) {
    	ret = cmd.existholder();
    } else if ( prefix == PREFIXMODHOLDER) {
        ret = cmd.modholder();
    } else if ( prefix == PREFIXDELHOLDER) {
        ret = cmd.delholder();
    } else {
		throw UnknownCommandError();
	}
	
	return ret;
}
