#include "ServiceDispatcher.h"

namespace apiserver {

class ServiceDispatcher::exec_func
{

};

//注册服务
void ServiceDispatcher::registerService(ServiceWrapper* wrapper)
{

}

void ServiceDispatcher::dispatchMessage(MessageContext* context)
{
	//找到对应的服务名称
	//找到操作名
	//解析请求uri中的参数
}


}