#include "Log.h"
#include <time.h>
#include <sys/timeb.h>

namespace apiserver {

Log::Log(const char* log_filename)
{
	static std::ofstream of;//此时of是一个局部变量
	of.open(log_filename);
	ofstream = &of;
}

std::ofstream* Log::write_log(LogLevel log_level, const char* log_str)
{
	struct tm localtime;
	struct timeb timeBuf;
	ftime(&timeBuf);
	localtime_r(&timeBuf.time, &localtime);
	static const unsigned int buffSize = 32;
	char buff[buffSize] = "\0";
	snprintf(buff, buffSize, "%02d-%02d-%02d %02d:%02d:%02d.%03d ",
					localtime.tm_year + 1900, localtime.tm_mon + 1, localtime.tm_mday,
					localtime.tm_hour, localtime.tm_min, localtime.tm_sec, timeBuf.millitm);
	(*ofstream) << buff << EnumToString(log_level) << log_str << std::endl;
	return ofstream;
}

}