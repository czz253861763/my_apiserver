#ifndef CLIENTCALLBACK
#define CLIENTCALLBACK

namespace apiserver {

//客户端事件回调类，用于执行客户端事件回调
class ClientCallback
{
public:
	virtual void connected(int new_client_socket, struct sockaddr_storage* client_addr) = 0;
	virtual bool readyRead(int client_socket) = 0;
	virtual void showClientInfo() = 0;
};

}

#endif