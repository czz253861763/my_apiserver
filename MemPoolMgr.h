#ifndef MEMPOOLMGR
#define MEMPOOLMGR

#include "MemPool.h"

namespace apiserver {

struct Pools
{
	std::std::vector<MemPool*> used;
	std::std::vector<MemPool*> unused;
};

class MemPoolMgr
{
public:
	MemPoolMgr();
	~MemPoolMgr();
	MemPool* get(uint64_t chunkSize = DEFAULT_MEM_SIZE);
	void recycle(MemPool* mempool);
private:
	std::unordered_map<uint64_t, Pools> pools_map;
};

}

#endif