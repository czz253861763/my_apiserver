#ifndef SERVER
#define SERVER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/epoll.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>

#include "ClientCallback.h"

namespace apiserver {

typedef int Socket;

class Server {
public:
    Server(std::string port);//构造初始化
    ~Server();
    //bool create();
    bool handleIncomingConnection();
    bool setSocketNonblock(int fd);
    bool setClientCallback(ClientCallback* clientcallback);
    int start();//启动服务端
    int stop();//停止服务端

private:
    int listen_fd;
    int epoll_fd;
    struct epoll_event event;
    struct epoll_event* events;

    std::string ip;
    std::string listen_port;

    ClientCallback* clientcallback;
};




}

#endif