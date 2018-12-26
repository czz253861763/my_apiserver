#include "MemPool.h"
#include "Log.h"
#include <unistd.h>
#include "server.h"
#include "ClientHandler.h"

int main(int argc, char** argv)
{
	
	std::string listen_port = "3490";

	apiserver::ClientHandler clienthandler;

	apiserver::Server apiserver(listen_port);
	apiserver.setClientCallback(&clienthandler);//设置客户端消息处理程序
	apiserver.start();
	usleep(60 * 1000 * 1000);
	
	apiserver::MemPool* mempool_ptr = new apiserver::MemPool();
	uchar* buffer = mempool_ptr->grow(500);
	printf("buffer addr:%d\n", (unsigned long)(buffer));
	std::cout << "hello" << std::endl;
	return 0;
}
