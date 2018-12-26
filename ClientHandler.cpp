#include "ClientHandler.h"

#define READ_BLOCK 512

namespace apiserver {

ClientInfo::ClientInfo(): client_fd(-1), httpBodyRemaining(INVALID), httpBodyOffset(INVALID), poolRequest(new MemPool()), poolResponse(new MemPool())
{

}

ClientInfo::~ClientInfo()
{

}

ClientHandler::ClientHandler(): pool(new MemPool()) {}

void ClientHandler::connected(int new_client_socket, struct sockaddr_storage* client_addr)
{
	//保存新建立的连接客户端信息
	clients[new_client_socket] = new ClientInfo();

	ClientInfo* clientinfo = clients[new_client_socket];//通过key来访问
	clientinfo->client_fd = new_client_socket;

	//读取结构体客户端信息并保存到unordered_map
	socklen_t client_len = sizeof(struct ::sockaddr_storage);//在作用域中需要全局访问
	//socklen_t client_len = sizeof(struct sockaddr_in);

	int res = getnameinfo((struct sockaddr *)client_addr, client_len, 
							clientinfo->hoststr, sizeof(clientinfo->hoststr), 
							clientinfo->portstr, sizeof(clientinfo->portstr), 
    						NI_NUMERICHOST | NI_NUMERICSERV);
	printf("getnameinfo res:%s\n", gai_strerror(res));
}

bool ClientHandler::readyRead(int client_socket)
{
	auto it = clients.find(client_socket);
	if(it == clients.end())
	{
		return false;
	}

	ClientInfo* clientinfo = it->second;

	while(1)//边沿触发，需要循环recv
	{
		uint64_t readSize = (clientinfo->httpBodyRemaining != INVALID) ? 
							clientinfo->httpBodyRemaining : READ_BLOCK;

		uchar* buffer = (clientinfo->poolRequest)->grow(readSize);//内存池分配内存

		int recv_len = ::recv(clientinfo->client_fd, buffer, readSize, 0);
		printf("recv_len:%d\n", recv_len);
		if(recv_len == -1)
		{
			printf("errno:%s\n", strerror(errno));
			if(errno != EAGAIN)
			{
				return false;
			}
			break;
			//回收内存池上一次分配的内存
			//(clientinfo->poolRequest)->shrink(readSize);
		}
		else if(recv_len == 0)
		{
			//客户端关闭
			return false;
		}
		else if(recv_len > 0)
		{
			if(recv_len < readSize)
			{
				//回收内存池多余内存，不进行回收

			}
			else if(recv_len == readSize)
			{

			}
			if(clientinfo->httpBodyRemaining != INVALID)
			{
				clientinfo->httpBodyRemaining -= recv_len;
			}
		}
		else
		{

		}
	}
}

void ClientHandler::showClientInfo()
{
	for(auto it = clients.begin(); it != clients.end(); it++)
	{
		std::cout << it->first << std::endl;
		//std::cout << (unsigned long)it->second << std::endl;
		printf("hoststr:%s\n", (it->second)->hoststr);
		printf("portstr:%s\n", (it->second)->portstr);
	}
}

}
