#include "Log.h"

namespace apiserver {

Log::Log(std::string log_filename): std::ofstream(log_filename, std::ios::app) {}

std::ostream& operator<<(std::ostream& ostream, std::string log_str)
{
	ostream << log_str;
	return ostream;
}

}