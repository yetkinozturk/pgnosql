//LOG options 
#define FILEONLY 1
#define CONSOLEONLY 2

#define LOGMODE CONSOLEONLY

#define LOGGIN_CONFIG \
do { \
	if (LOGMODE == CONSOLEONLY) { \
		PropertyConfigurator::configure("config/logconsole.cfg"); \
	} \
	else { \
		PropertyConfigurator::configure("config/logfile.cfg"); \
	} \
} while (0);


