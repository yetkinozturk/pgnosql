#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__

#include <iostream>
#include <map>
#include <string>
#include <libconfig.h++>
using namespace libconfig;

std::map<std::string,std::string> getConfiguration();

#endif
