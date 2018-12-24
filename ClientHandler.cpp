#include "ClientHandler.h"

#define READ_BLOCK 512

namespace apiserver {

ClientHandler::ClientHandler(): pool(new MemPool()) {}

void ClientHandler::connected(int new_client_socket, struct sockaddr_storage* client_addr)
{
	//保存新建立的连接客户端信息
	ClientInfo* clientinfo = clients[new_client_socket];//通过key来访问
	//读取结构体客户端信息并保存到unordered_map
	socklen_t client_len = sizeof(struct ::sockaddr_storage);//在作用域中需要全局访问
	char hoststr[NI_MAXHOST];
	char portstr[NI_MAXSERV];
	int res = getnameinfo((struct sockaddr *)client_addr, 
    						client_len, hoststr, sizeof(hoststr), portstr, sizeof(portstr), 
    						NI_NUMERICHOST | NI_NUMERICSERV);
	if (res == 0) 
    	printf("New connection from %s %s", hoststr, portstr);
}

bool ClientHandler::readyRead(int client_socket)
{
	/*
	auto it = clients.find(client_socket);
	if(it == clients.end())
	{
		return false;
	}
	const ClientInfo* clientinfo = it->second;

	while(1)//边沿触发，需要循环recv
	{
		uint64_t readSize = (clientinfo->httpBodyRemaining > 0) ? clientinfo->httpBodyRemaining : READ_BLOCK;
		buffer = poolRequest->MemPool_malloc(readSize);//内存池分配内存
		int recv_len = ::recv(clientinfo->client_fd, buffer, readSize, 0);
		if(recv_len == -1)
		{
			if(errno != EAGAIN)
			{
				return false;
			}
			//回收内存池上一次分配的内存
		}
		else if(recv_len == 0)
		{
			//客户端关闭
			return false;
		}
		else if(recv_len > 0)
		{
			if(recv_len < readyRead)
			{
				//回收内存池多余内存

			}
			if(clientinfo->httpBodyRemaining > 0)
			{
				clientinfo->httpBodyRemaining -= recv_len;
			}
		}
		else
		{

		}
	}
	*/
}

void ClientHandler::showClientInfo()
{
	for(auto it = clients.begin(); it != clients.end(); it++)
	{
		std::cout << it->first << std::endl;
		std::cout << it->second << std::endl;
	}
}

}
