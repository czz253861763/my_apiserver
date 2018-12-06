#include "MemPool.h"
#include <unistd.h>

int main(int argc, char** argv)
{
	int i = 0;
	apiserver::MemPool mempool(7 * 1024);
	mempool.MemPool_malloc(2 * 1024);
	mempool.MemPool_malloc(5 * 1024);
	mempool.MemPool_malloc(5 * 1024);
	mempool.MemPool_malloc(3 * 1024);
	mempool.MemPool_malloc(3 * 1024);
	mempool.MemPool_malloc(3 * 1024);
	mempool.MemPool_malloc(3 * 1024);

	return 0;
}