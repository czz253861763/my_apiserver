#ifndef MEMPOOL
#define MEMPOOL

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

typedef unsigned char uchar;
#define DEFAULT_MEM_SIZE (4 * 1024)//默认4k大小

namespace apiserver {

struct Chunk
{
	uchar* buffer;
	uint64_t bufferSize;//内存块总大小
	uint64_t size;//内存块当前的大小
};

class MemPool
{
public:
	MemPool(uint64_t chunkSize = DEFAULT_MEM_SIZE);
	~MemPool();
	bool newChunk(uint64_t newChunkSize);
	//内联函数
	inline uchar* grow(uint64_t growSize)
	{
		uint64_t offset = 0;
		if(curChunk == nullptr || (curChunk->size + growSize) > curChunk->bufferSize)
		{
			offset = 0;
			newChunk(growSize > chunkSize ? growSize : chunkSize);
			curChunk->size = growSize;
		}
		else
		{
			//curChunk有足够空间
			offset = curChunk->size;
			curChunk->size += growSize;
		}
		
		return (curChunk->buffer + offset);
	}
	bool shrink(uint64_t shrink_size);
//private:
	const uint64_t chunkSize;//保存默认初始化chunkSize大小
	Chunk* chunkHead;//指向当前内存头
	Chunk* curChunk;//指向最后一个已使用的内存块
	int chunksLeft;
};


}

#endif
