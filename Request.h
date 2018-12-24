#ifndef REQUEST
#define REQUEST

#include "MemPool.h"
#include "Engine.h"

namespace apiserver {

//Header用链表的结构
struct Header
{
	const char* name = nullptr;
	const char* value = nullptr;
	Header* next = nullptr;
};

//request数据结构体，c++11直接赋值
struct Request
{
	const char* path = nullptr;//REST资源的路径
	Header* headers = nullptr;//请求头部
	char* body = nullptr;
	uint64_t bodySize = 0;
	MemPool* mempool = nullptr;
	//Node* node = nullptr;

	//结构体成员函数
	const Header* getHeader(const char* name) const;
};

struct Response
{
	Header* headers = nullptr;
	//Node* node = nullptr;
	MemPool* mempool = nullptr;
};

//客户端消息用于分发的结构体
/*
struct MessageContext
{
	Engine* engine = nullptr;
	Transport* transport = nullptr;
	Request* request = nullptr;
	Response* response = nullptr;
	MessageCallback* callback = nullptr;
	MemPool* pool = nullptr;
};
*/
}
#endif