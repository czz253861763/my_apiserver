#ifndef LOG
#define LOG

#include <fstream>
#include <iostream>
#include <string>

namespace apiserver {
//继承ofstream写文件类
class Log: public std::ofstream
{
public:
	explicit Log(std::string log_filename);
	friend std::ofstream& operator<<(std::ofstream& ofstream, std::string log_str);
};

}
#endif