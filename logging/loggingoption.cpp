#include "loggingoption.h"

LoggingOption::LoggingOption()
{
	fileLogging = false;
	consoleLogging = true;
}

LoggingOption::LoggingOption(bool fileLogging, bool consoleLogging)
{
	fileLogging = fileLogging;
	consoleLogging = consoleLogging;
}

LoggingOption::~LoggingOption(){}
