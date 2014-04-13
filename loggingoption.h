#ifndef _LOGGING_OPTION_H
#define _LOGGING_OPTION_H

#define LOGGIN_CONFIG \
do { \
    if (fileLogging && consoleLogging) { \
        PropertyConfigurator::configure("config/logconsolefile.cfg"); \
    } \
    else if (fileLogging){ \
        PropertyConfigurator::configure("config/logfile.cfg"); \
    } \
    else { \
        PropertyConfigurator::configure("config/logconsole.cfg"); \
    }\
} while (0);

class LoggingOption 
{
public:
	bool fileLogging;
	bool consoleLogging;
	LoggingOption();
	LoggingOption(bool fileLogging, bool consoleLogging);
	~LoggingOption();
};
#endif
