#ifndef LOG
#define LOG

#include <fstream>
#include <iostream>
#include <string>

namespace apiserver {

enum LogLevel
{
	debug,
	info,
	error
};

inline const char* EnumToString(LogLevel loglevel)
{
    switch(loglevel)
    {
        case debug:
            return "[DEBUG] ";
        case info:
            return "[INFO] ";
        case error:
            return "[ERROR] ";
        default:
            return "[NONE] ";
    }
}

class Log
{
public:
	Log(const char* log_filename);
	std::ofstream* write_log(LogLevel log_level, const char* log_str);
private:
	std::ofstream* ofstream;
};

}
#endif