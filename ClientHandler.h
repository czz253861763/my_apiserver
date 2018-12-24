#ifndef CLIENTHANDLER
#define CLIENTHANDLER

#include <string>
//#include "HttpRequest.h"
#include "MemPool.h"
#include "ClientCallback.h"
#include <sys/socket.h>
#include <netdb.h>

namespace apiserver {

//保存客户端信息
class ClientInfo
{
public:
	ClientInfo();
	~ClientInfo();
	int client_fd;//client socket
	std::string host;
	std::string port;
	//HttpRequest request;
	//HttpResponse response;
	//uint64_t httpBodyRemaining;
	//uint64_t httpBodyOffset;//保存接收到http消息体偏移
	//MemPool* poolRequest;
	//MemPool* poolResponse;
};

class ClientHandler: public ClientCallback
{
public:
	ClientHandler();
	virtual void connected(int new_client_socket, struct sockaddr_storage* client_addr) override;//处理新连接建立事件
	virtual bool readyRead(int client_socket) override;//读数据
	virtual void showClientInfo();
private:
	std::unordered_map<int, ClientInfo*> clients;//保存已连接的客户端信息
	MemPool* pool;//用于处理客户端的内存池
};

}

#endif
