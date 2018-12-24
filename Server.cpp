#include "server.h"
#define BACKLOG 10
#define EPOLL_MAX_NUM 2048

#include <fcntl.h>
#include <netinet/tcp.h>

namespace apiserver {

Server::Server(std::string port): 
	listen_port(port), listen_fd(-1), events(nullptr), clientcallback(nullptr) {}

Server::~Server()
{
	close(listen_fd);
	close(epoll_fd);
}

int Server::start()
{
	struct addrinfo hints;
	struct addrinfo* serverinfo;
	struct addrinfo* cur;
	int reuse = 1;
	int rv;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if((rv = getaddrinfo(nullptr, listen_port.c_str(), &hints, &serverinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	}

	for(cur = serverinfo; cur != nullptr; cur = cur->ai_next)
	{
		if((listen_fd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol)) == -1)
		{
			perror("server: socket");
			exit(1);
		}
		if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)))
		{
			perror("setsocketopt");
			exit(1);
		}
		if(bind(listen_fd, cur->ai_addr, cur->ai_addrlen) == -1)
		{
			perror("server: bind");
			close(listen_fd);
			continue;
		}
		break;
	}

	if(cur == nullptr)
	{
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	freeaddrinfo(serverinfo);

	if(listen(listen_fd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}
	
	epoll_fd = epoll_create(EPOLL_MAX_NUM);
	if(epoll_fd < 0)
	{
		perror("epoll_create");
		exit(1);
	}

	event.events = EPOLLIN | EPOLLET;
	event.data.fd = listen_fd;

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) < 0)
	{
		perror("epoll ctl add listen_fd");
	}
	events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * EPOLL_MAX_NUM);

	while(1)
	{
		int active_fds_cnt = epoll_wait(epoll_fd, events, EPOLL_MAX_NUM, -1);
		for(int i = 0; i < active_fds_cnt; i++)
		{
			if(events[i].data.fd & EPOLLHUP)
			{

			}
			else if(events[i].data.fd == listen_fd)
			{
				//建立新连接
				bool res = handleIncomingConnection();
			}
			else
			{
				if(events[i].events & EPOLLIN)
				{
					//已建立的连接收到数据
				}
				else if(events[i].events & EPOLLOUT)
				{
					//已建立的连接可以发送数据
				}
				else
				{

				}
			}
		}
	}
}

bool Server::setSocketNonblock(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	int res = fcntl(fd, F_SETFL, 0);
	if(res != -1)
		return true;
	else
		return false;
}

bool Server::handleIncomingConnection()
{
	//处理新建立的连接
	struct sockaddr_storage client_addr;
	socklen_t sin_size;
	int new_client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &sin_size);
	int res = setSocketNonblock(new_client_fd);//将客户端socket设置成非阻塞
	int nodelayOpt = 1;
	setsockopt(new_client_fd, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&nodelayOpt), sizeof(nodelayOpt));

	event.data.fd = new_client_fd;
	event.events = EPOLLIN | EPOLLOUT | EPOLLET;
	res = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_client_fd, &event);

	//send(new_client_fd, "Hello, world!", 13, 0);
	//将客户端信息加入客户端信息结构体，通过回调函数
	clientcallback->connected(new_client_fd, &client_addr);
	clientcallback->showClientInfo();
}

bool Server::setClientCallback(ClientCallback* clientcallback)
{
	this->clientcallback = clientcallback;
	return true;
}

}