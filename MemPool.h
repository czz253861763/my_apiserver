#ifndef MEMPOOL
#define MEMPOOL

#include <iostream>
#include <vector>
#include <unordered_map>

typedef unsigned char uchar;
#define MAX_ALLOC_FROM_POOL (4*1024)//单个内存页大小4k
typedef void (*pool_cleanup_ptr)(void* data);

namespace apiserver {

struct MemPoolHeader_base;
struct MemPoolHeader_ext;
struct MemChunk_large;
struct MemPool_cleanup;

//头部base
struct MemPoolHeader_base
{
	uchar* last;
	uchar* end;
	MemPoolHeader_base* next;
	unsigned short failed_count;
};

//头部ext
struct MemPoolHeader_ext
{
	MemPoolHeader_base base;
	MemChunk_large* large;
	MemPool_cleanup* cleanup;//销毁内存池时调用的清理函数
};

//大内存块
struct MemChunk_large
{
	uchar* ptr;
	MemChunk_large* next;
};

struct MemPool_cleanup
{
	pool_cleanup_ptr handler;
	void* data;
	MemPool_cleanup* next;
};

class MemPool
{
public:
	MemPool(size_t size = (4 * 1024));
	uchar* MemPool_malloc(size_t size);
	uchar* MemPool_malloc_large(size_t size);
	uchar* MemPool_malloc_small(MemPoolHeader_base* ptr, size_t size);
	void MemPool_reset(bool flag_destroy = false);
private:
	MemPoolHeader_ext* pool;
	size_t init_size;
};

}

#endif