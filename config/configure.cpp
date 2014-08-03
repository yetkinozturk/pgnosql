#include <iostream>
#include <map>
#include "configure.h"

std::map<std::string,std::string> getConfiguration()
{
   Config cfg;
   std::map<std::string,std::string> config;
   try
   {
      cfg.readFile("config/pgnosql.cfg");
   }
   catch(const FileIOException &fioex)
   {
        std::cerr << "I/O error while reading config/pgnosql.cfg" << std::endl;
        exit(EXIT_FAILURE);
   }
   catch(const ParseException &pex)
   {
        std::cerr << "Configuration file parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << std::endl;
        exit(EXIT_FAILURE);
   }

   try
   {
        const Setting& root = cfg.getRoot();
        const Setting &pgsql = root["pgnosql"]["postgresql"];
        const Setting &network = root["pgnosql"]["network"];
		const Setting &logging = root["pgnosql"]["logging"];

        if (!(pgsql.lookupValue("username", config["pg_username"]) &&
              pgsql.lookupValue("password", config["pg_password"]) &&
              pgsql.lookupValue("hostname", config["pg_hostname"]) &&
              pgsql.lookupValue("dbname", config["pg_dbname"]) &&
              pgsql.lookupValue("port", config["pg_port"])))
        {
			std::cerr<<"Error while getting postgresql config from config/pgnosql.cfg"<<std::endl;
        }
        if (!(network.lookupValue("username", config["username"]) &&
              network.lookupValue("password", config["password"]) &&
              network.lookupValue("port", config["port"]) &&
			  network.lookupValue("connection_limit",config["connection_limit"])))
        {
            std::cerr<<"Error while getting network config from config/pgnosql.cfg"<<std::endl;
        }
        if (!(logging.lookupValue("filelog", config["filelog"]) &&
              logging.lookupValue("consolelog", config["consolelog"])))
        {
            std::cerr<<"Error while getting logging config from config/pgnosql.cfg"<<std::endl;
        }
   }
   catch(const SettingNotFoundException &nfex)
   {
        std::cerr<<"missing configuration at config/pgnosql.cfg"<<std::endl;
   }
   return config;
}

