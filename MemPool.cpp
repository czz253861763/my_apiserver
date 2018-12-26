#include "MemPool.h"

#define DEFAULT_CHUNK_NUM 64

namespace apiserver {
//构造函数并不分配内存，使用时再分配
MemPool::MemPool(uint64_t chunkSize): chunkSize(chunkSize), chunkHead(nullptr), curChunk(nullptr), chunksLeft(0) {}

bool MemPool::newChunk(uint64_t size)
{
	if(curChunk == nullptr)
	{
		Chunk* newChunkHead = (Chunk*)malloc(sizeof(Chunk) * DEFAULT_CHUNK_NUM);
		memset(newChunkHead, 0, sizeof(Chunk) * DEFAULT_CHUNK_NUM);
		chunkHead = newChunkHead;
		curChunk = newChunkHead;
		curChunk->buffer = (uchar*)malloc(size);
		curChunk->size = size;
		curChunk->bufferSize = size;
		chunksLeft = DEFAULT_CHUNK_NUM;
		return true;
	}
	else
	{
		if(chunksLeft == 0)
		{
			int chunknum = curChunk - chunkHead + 1;
			Chunk* newChunkHead = (Chunk*)realloc(chunkHead, sizeof(Chunk) * (chunknum + DEFAULT_CHUNK_NUM));
			memset(newChunkHead + chunknum, 0, sizeof(Chunk) * DEFAULT_CHUNK_NUM);
			chunksLeft = DEFAULT_CHUNK_NUM;
			chunkHead = newChunkHead;
			curChunk = chunkHead + chunknum;
		}
		else
			++curChunk;

		--chunksLeft;

		if(size > curChunk->bufferSize)
		{
			curChunk->buffer = (uchar*)realloc(curChunk->buffer, size);
			curChunk->size = 0;
			curChunk->bufferSize = size;
		}
		return true;
	}



}

}
