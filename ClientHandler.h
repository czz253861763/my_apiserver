#ifndef CLIENTHANDLER
#define CLIENTHANDLER

#include <iostream>
#include <string>

//#include "HttpRequest.h"
#include "MemPool.h"
#include "ClientCallback.h"
#include <sys/socket.h>
#include <netdb.h>
#include <unordered_map>

#include <string.h>
#include <stdio.h>

#define INVALID (static_cast<uint64_t>(-1))

namespace apiserver {

enum SMState
{
	start,
	method,
	spaces_before_uri,
	service,
	service_method,
	done
};

//保存客户端信息
class ClientInfo
{
public:
	ClientInfo();
	~ClientInfo();
	int client_fd;//client socket
	char hoststr[NI_MAXHOST];//client host ip
	char portstr[NI_MAXSERV];//client port
	//HttpRequest request;
	//HttpResponse response;
	uint64_t httpBodyRemaining;
	uint64_t httpBodyOffset;//保存接收到http消息体偏移
	MemPool* poolRequest;//保存http请求的内存池
	MemPool* poolResponse;
	SMState request_state;
	std::string service;
	std::string service_method;
	std::unordered_map<std::string, std::string> service_method_param;
};

class ClientHandler: public ClientCallback
{
public:
	ClientHandler();
	virtual void connected(int new_client_socket, struct sockaddr_storage* client_addr) override;//处理新连接建立事件
	virtual bool readyRead(int client_socket) override;//读数据
	virtual void showClientInfo();
	bool tryParseHttpRequest(ClientInfo* clientinfo);
	bool parseHttpRequest(ClientInfo* clientinfo);
private:
	std::unordered_map<int, ClientInfo*> clients;//保存已连接的客户端信息
	MemPool* pool;//用于处理客户端的内存池
};

}

#endif
