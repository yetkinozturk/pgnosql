#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__

#include <string>
#include <libconfig.h++>
using namespace libconfig;

std::map<std::string,std::string> getConfiguration();

#endif
