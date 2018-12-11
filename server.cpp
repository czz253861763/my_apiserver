#include "MemPool.h"
#include "Log.h"
#include <unistd.h>

int main(int argc, char** argv)
{
	/*
	int i = 0;
	apiserver::MemPool mempool(7 * 1024);

	std::cout << "init mempool ok" << std::endl;
	usleep(15 * 1000 * 1000);
	while(i++ < 50)
	{
		mempool.MemPool_malloc(5 * 1024);
	}
	std::cout << "malloc large ok" << std::endl;
	i = 0;
	usleep(15 * 1000 * 1000);
	while(i++ < 50)
	{
		mempool.MemPool_malloc(3 * 1024);
	}
	std::cout << "malloc small ok" << std::endl;
	usleep(15 * 1000 * 1000);
	mempool.MemPool_reset();
	std::cout << "mempool reset ok" << std::endl;
	usleep(30 * 1000 * 1000);
	*/
    const char* log_filename = "log.txt";
	apiserver::Log log(log_filename);
	apiserver::LogLevel  loglevel = apiserver::info;
	const char* log_str = "write_log";
    log.write_log(loglevel, log_str);
	log.write_log(loglevel, log_str);
	log.write_log(loglevel, log_str);

	//LogInfo() << "test_str";

	std::cout << "hello" << std::endl;
	return 0;
}
