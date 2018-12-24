#ifndef HTTPREQUEST
#define HTTPREQUEST

#include "Request.h"

namespace apiserver {
//定义http协议相关数据结构
enum class HttpMethod
{
	UNKNOWN,
	POST,
	GET,
	PUT,
	DELETE
};

enum class ContentType
{
	Unknown,
	NotSet,
	ApplicationJson
};

enum HttpStatus
{
	HTTP_STATUS_UNDEFINED,
	HTTP_STATUS_OK = 200,
	HTTP_STATUS_400_BAD_REQUEST = 400,
	HTTP_STATUS_404_NOT_FOUND = 404,
	HTTP_STATUS_500_INTERNAL_SERVER_ERROR = 500
};

struct HttpRequest : public Request
{
	HttpMethod method = HttpMethod::UNKNOWN;
	const char* methodStr = nullptr;
	const char* clientHost = nullptr;
	const char* clientPort = nullptr;
	ContentType ContentType = ContentType::NotSet;
};

struct HttpResponse : public HttpResponse
{
	int statusCode = HTTP_STATUS_UNDEFINED;
};

}



#endif