#ifndef MEMPOOL
#define MEMPOOL

#include <iostream>
#include <vector>
#include <unordered_map>

typedef unsigned char uchar;
#define MAX_ALLOC_FROM_POOL (4*1024)//单个内存页大小4k

namespace apiserver {

struct MemPoolHeader_base;
struct MemPoolHeader_ext;
struct MemChunk_large;

//头部base
struct MemPoolHeader_base
{
	uchar* last;
	uchar* end;
	MemPoolHeader_base* next;
};

//头部ext
struct MemPoolHeader_ext
{
	MemPoolHeader_base base;
	MemChunk_large* large;
};

//大内存块
struct MemChunk_large
{
	uchar* ptr;
	MemChunk_large* next;
};

class MemPool
{
public:
	MemPool(size_t size);
	uchar* MemPool_malloc(size_t size);
	uchar* MemPool_malloc_large(size_t size);
	uchar* MemPool_malloc_small(MemPoolHeader_base* ptr, size_t size);
private:
	MemPoolHeader_ext* pool;
};

}

#endif