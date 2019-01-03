#include "ClientHandler.h"

#define READ_BLOCK 512

namespace apiserver {

static uchar* findStr(uchar* buffer, uint64_t buffer_len, const char* substr, uint64_t substr_len)
{
	if(buffer == nullptr || substr == nullptr)
		return nullptr;
	if(substr_len > buffer_len)
		return nullptr;
	for(int i = 0; i <= buffer_len - substr_len; i++)
	{
		int j = 0;
		for(j = 0; j < substr_len; j++)
		{
			if(substr[j] != buffer[i + j])
				break;
		}
		if(j == substr_len)
		{
			return &(buffer[i]);
		}
	}
	return nullptr;
}

ClientInfo::ClientInfo(): 
	client_fd(-1), httpBodyRemaining(INVALID), httpBodyOffset(INVALID), 
	poolRequest(new MemPool()), poolResponse(new MemPool()), 
	request_state(start) {}

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
				(clientinfo->poolRequest)->shrink(readSize);
				break;
			}
			else
			{
				return false;
			}
		}
		else if(recv_len == 0)
		{
			//客户端关闭
			return false;
		}
		else if(recv_len > 0)
		{
			//已经将读取的数据存入内存池Chunk
			/*
			printf("recv_buffer:%s\n", buffer);//打印http请求
			for(int i = 1; i <= 10; i++)
				printf("recv:%d\n", buffer[recv_len - i]);
			*/
			//http请求头和body直接CRLFCRLF分隔

			if(recv_len < readSize)
			{
				//回收内存池多余内存，不进行回收
				(clientinfo->poolRequest)->shrink(readSize - recv_len);

			}
			else if(recv_len == readSize)
			{

			}
			/*
			if(clientinfo->httpBodyRemaining != INVALID)
			{
				clientinfo->httpBodyRemaining -= recv_len;
			}
			*/
			bool res = tryParseHttpRequest(clientinfo);

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

bool ClientHandler::tryParseHttpRequest(ClientInfo* clientinfo)
{
	//在内存池curChunk中解析/r/n/r/n字符，可能会存在同一个Chunk解析多次
	MemPool* pool = clientinfo->poolRequest;
	Chunk* curChunk = pool->curChunk;//得到内存池当前Chunk的指针
	uint64_t size = curChunk->size;//当前Chunk使用大小
	printf("tryParseHttpRequest_curChunk:%d\n", (uint64_t)curChunk);
	printf("tryParseHttpRequest_size:%d\n", size);

	uchar* res = findStr(curChunk->buffer, size, "\r\n\r\n", 4);//实现效率不高，待优化？？
	/*
	if(res != nullptr)
	{
		uchar* tmp_ptr = curChunk->buffer;
		for(tmp_ptr = curChunk->buffer; tmp_ptr != res; ++tmp_ptr)
		{
			printf("%c", (*tmp_ptr));
		}
	}
	*/
	clientinfo->httpBodyOffset = (res - curChunk->buffer + 1 + 4);

	if(res == nullptr)
		return false;

	//需要使用状态机解析http请求头部

	bool parse_res = parseHttpRequest(clientinfo);
	if(parse_res == false)
		return false;

	return true;
}

bool ClientHandler::parseHttpRequest(ClientInfo* clientinfo)
{
	uchar* tmp_ptr = ((clientinfo->poolRequest)->curChunk)->buffer;//请求内存起始地址
	uchar* end = tmp_ptr + ((clientinfo->poolRequest)->curChunk)->size;
	//printf("tmp_ptr:%d\n", (unsigned long)tmp_ptr);
	//printf("end:%d\n", (unsigned long)end);
	//printf("recv:%s\n", tmp_ptr);
	SMState tmp_state = (clientinfo->request_state);
	//printf("tmp_state:%d\n", tmp_state);
	uchar* service_ptr = nullptr;
	int service_len = 0;
	uchar* service_method_ptr = nullptr;
	int service_method_len = 0;

	while(tmp_ptr != end && tmp_state != done)
	{
		//printf("tmp_ptr:%d value:%c tmp_state:%d\n", (unsigned long)tmp_ptr, *tmp_ptr, tmp_state);
		switch(tmp_state)
		{
			case start:
			{
				//获取method之前预处理
				tmp_state = method;
				break;
			}
			case method:
			{
				if((*tmp_ptr == 'G') && (*(tmp_ptr + 1) == 'E') && (*(tmp_ptr + 2) == 'T'))
				{
					tmp_ptr = tmp_ptr + 3;
					tmp_state = spaces_before_uri;
				}
				else if((*tmp_ptr == 'P') && (*(tmp_ptr + 1) == 'O') && (*(tmp_ptr + 2) == 'S') && (*(tmp_ptr + 3) == 'T'))
				{
					tmp_ptr = tmp_ptr + 4;
					tmp_state = spaces_before_uri;
				}
				else
				{

				}
				break;
			}
			case spaces_before_uri:
			{
				//跳过空格
				if(*tmp_ptr == '/')
				{
					tmp_state = service;
				}
				++tmp_ptr;
				break;
			}
			case service:
			{
				//合适获取服务停止？
				if(*tmp_ptr != '/' && service_len == 0)
				{
					service_ptr = tmp_ptr;
					++service_len;
				}
				else if(*tmp_ptr != '/')
				{
					++service_len;
				}
				else
				{
					tmp_state = service_method;
				}
				++tmp_ptr;//跳过了下一个/
				break;

			}
			case service_method:
			{
				if(*tmp_ptr != ' ' && service_method_len == 0)
				{
					service_method_ptr = tmp_ptr;
					++service_method_len;			
				}
				else if(*tmp_ptr != ' ')
				{
					++service_method_len;
				}
				else
				{
					tmp_state = done;
				}
				++tmp_ptr;//跳过了结束符空格
				break;
			}
			default:
			{

			}
		}
	}
	//转换服务名保存到clientinfo->info中
	char service_arr[service_len + 1];
	for(int i = 0; i < service_len; i++)
	{
		service_arr[i] = *(service_ptr + i);
	}
	service_arr[service_len] = '\0';
	std::string service_str(service_arr);
	clientinfo->service = service_str;
	//解析参数并转换参数格式
	char service_method_arr[service_method_len + 1];
	for(int i = 0; i < service_method_len; i++)
	{
		service_method_arr[i] = *(service_method_ptr + i);
	}
	service_method_arr[service_method_len] = '\0';

	const char* sep = "?&";
	char* p = strtok(service_method_arr, sep);
	int count = 0;
	while(p)
	{
		//printf("strtok:%s\n", p);
		if(count == 0)
		{
			std::string service_method_str(p);
			clientinfo->service_method = service_method_str;
			++count;
		}
		else
		{
			//解析参数
			//std::cout << "strlen:" << strlen(p) << std::endl;
			//printf("strtok:%s\n", p);
			
			char tmp_arr[strlen(p) + 1];
			tmp_arr[strlen(p)] = '\0';
			strncpy(tmp_arr, p, strlen(p));
			
			char* res_ptr = strstr(tmp_arr, "=");
			char param_name_arr[strlen(p) + 1];
			char param_value_arr[strlen(p) + 1];
			strncpy(param_name_arr, tmp_arr, (res_ptr - tmp_arr));
			param_name_arr[res_ptr - tmp_arr] = '\0';//下标从0开始
			strncpy(param_value_arr, res_ptr + 1, tmp_arr + strlen(p) - 1 - res_ptr);
			param_value_arr[tmp_arr + strlen(p) - 1 - res_ptr] = '\0';//下标从0开始

			std::string param_name = param_name_arr;
			std::string param_value = param_value_arr;
			(clientinfo->service_method_param)[param_name] = param_value;
		}
		p = strtok(NULL, sep);
	}
	//std::cout << "clientinfo->service:" << clientinfo->service << std::endl;
	//printf("service_len:%d\n", service_len);
	//printf("service_method_len:%d\n", service_method_len);
	//printf("service:%.*s\n", service_len, service_ptr);
	//printf("service_method:%.*s\n", service_method_len, service_method_ptr);

	for(auto it = (clientinfo->service_method_param).begin(); it != (clientinfo->service_method_param).end(); it++)
	{
		std::cout << "param_name:" << it->first << std::endl;
		std::cout << "param_value:" << it->second << std::endl;
	}
}

}
