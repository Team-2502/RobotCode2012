#include <cstdarg>
#include <cstdio>
#include "time.h"
#include "sysLib.h" //For sysClkRateGet()
#include "Timer.h"
#include "Logger.h"

using std::clock;
using std::endl;
using std::string;

Logger::Logger(const string& fileName)
{
	this->file.open(fileName.c_str());
}

Logger::~Logger()
{
	this->file.close();
}

void Logger::Logf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char buffer[256];
	vsnprintf(buffer, 255, format, args);
	va_end(args);
	
	//Create a timestamp
	char message[300];
	double time = (double)GetClock() / (double)CLOCKS_PER_SEC;
	sprintf(message, "[%f] %s", time, buffer);
	
	this->file << message << endl;
}
