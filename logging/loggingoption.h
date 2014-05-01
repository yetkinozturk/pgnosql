#ifndef _LOGGING_OPTION_H
#define _LOGGING_OPTION_H

#define LOGGING_CONFIG \
do { \
    if (logOption.fileLogging && logOption.consoleLogging) { \
        PropertyConfigurator::configure("config/logconsolefile.cfg"); \
    } \
    else if (logOption.fileLogging){ \
        PropertyConfigurator::configure("config/logfile.cfg"); \
    } \
    else { \
        PropertyConfigurator::configure("config/logconsole.cfg"); \
    }\
} while (0);

struct LoggingOption 
{
	bool fileLogging;
	bool consoleLogging;
};
#endif
