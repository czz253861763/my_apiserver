#include "MemPool.h"

namespace apiserver {

MemPool::MemPool(size_t size)
{
	pool = (MemPoolHeader_ext*)malloc(size + sizeof(MemPoolHeader_ext));
	if(pool == nullptr)
		std::cout << "malloc error" << std::endl;
	pool->base.last = (uchar*)pool + sizeof(MemPoolHeader_ext);
	pool->base.end = (uchar*)pool + size + sizeof(MemPoolHeader_ext);
	//std::cout << std::hex << (unsigned long)pool << std::endl;
	//std::cout << std::hex << (unsigned long)pool->base.last << std::endl;
	//std::cout << std::hex << (unsigned long)pool->base.end << std::endl;
	pool->base.next = nullptr;
	pool->large = nullptr;
}

uchar* MemPool::MemPool_malloc(size_t size)
{
	uchar* ptr = nullptr;
	if(size > MAX_ALLOC_FROM_POOL)//在大内存链表中分配
	{
		ptr = MemPool_malloc_large(size);
	}
	else//在内存池小内存中分配
	{
		MemPoolHeader_base* tmp_ptr = &(*pool).base;

		uchar* res = nullptr;

		while(tmp_ptr)
		{
			res = MemPool_malloc_small(tmp_ptr, size);
			if(res != nullptr)
				return res;
			else
			{
				if(tmp_ptr->next != nullptr)
				{
					tmp_ptr = tmp_ptr->next;
				}
				else
				{
					tmp_ptr = nullptr;
				}
				//std::cout << "tmp_ptr" << (unsigned long)tmp_ptr << std::endl;
			}
		}

		if(res == nullptr)//遍历所有内存池块内存均不足，创建新内存池块
		{
			//分配新的内存池块
			MemPoolHeader_base* tmp_pool = (MemPoolHeader_base*)malloc(MAX_ALLOC_FROM_POOL + sizeof(MemPoolHeader_base));
			tmp_pool->last = (uchar*)tmp_pool + sizeof(MemPoolHeader_base);
			tmp_pool->end = (uchar*)tmp_pool + sizeof(MemPoolHeader_base) + MAX_ALLOC_FROM_POOL;
			tmp_pool->next = nullptr;
			std::cout << "create new mempool:" << size << "ok" << std::endl;
			std::cout << "last:" << (unsigned long)(tmp_pool->last) << std::endl;
			std::cout << "end:" << (unsigned long)(tmp_pool->end) << std::endl;
			//加上当前内存池块
			MemPoolHeader_base* tmp_ptr = &(*pool).base;
			while(tmp_ptr->next != nullptr)
			{
				tmp_ptr = tmp_ptr->next;
			}
			tmp_ptr->next = tmp_pool;
			res = MemPool_malloc_small(tmp_pool, size);
			std::cout << "res:" << (unsigned long)res << std::endl;
			return res;
		}
	}
}

uchar* MemPool::MemPool_malloc_large(size_t size)
{
	MemChunk_large* ptr = (MemChunk_large*)malloc(sizeof(MemChunk_large));
	ptr->next = nullptr;
	ptr->ptr = (uchar*)malloc(size);

	MemChunk_large* tmp_ptr = pool->large;
	//添加到大内存链表
	if(tmp_ptr == nullptr)
	{
		tmp_ptr = ptr;
	}
	else
	{
		while(tmp_ptr->next != nullptr)
		{
			tmp_ptr = tmp_ptr->next;
		}
		tmp_ptr->next = ptr;
	}
	std::cout << "alloc large mem:" << size << "ok" << std::endl;
	return ptr->ptr;
}
//在当前的内存池块上分配size大小的小内存
uchar* MemPool::MemPool_malloc_small(MemPoolHeader_base* ptr, size_t size)
{
	uchar* last = ptr->last;
	uchar* end = ptr->end;
	uchar* res = nullptr;
	//std::cout << "size:" << size << "last:" << (unsigned long)last << "end:" << (unsigned long)end << std::endl;
	//std::cout << "left:" << (size_t)(end - last) << std::endl;
	if(size <= (size_t)(end - last))//当前内存池内存充足
	{
		res = last;
		ptr->last = ptr->last + size;
		std::cout << "alloc small mem:" << size << "ok" << std::endl;
		//std::cout << "last:" << (unsigned long)last << std::endl;
	}
	return res;
}

}