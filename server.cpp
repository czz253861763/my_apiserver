#include "MemPool.h"
#include <unistd.h>

int main(int argc, char** argv)
{
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
	return 0;
}