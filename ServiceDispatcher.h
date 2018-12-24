#ifndef SERVICEDISPATCHER
#define SERVICEDISPATCHER

#include <string>
#include <vector>
#include <unordered_map>
#include "ServiceDesc.h"
#include "Request.h"

namespace apiserver {

//用户描述参数
struct Parameter
{
	std::string name;
	std::string divider;
};
//参数与操作描述结构体
struct Resource
{
	std::vector<Parameter> parameters;
	const OperationDescription* operation = nullptr;
};
//用于描述已部署的服务
struct DeployedService
{
	ServiceWrapper* servicewrapper = nullptr;
	std::unordered_multimap<std::string, Resource> staticResources;//静态资源？
	std::unordered_multimap<std::string, Resource> paramResources;//参数资源？
};

struct ResourcePath
{

	std::string name;
};

//负责deployment使用，用于注册服务，服务分发
class ServiceDispatcher
{
public:
	ServiceDispatcher();
	~ServiceDispatcher();

	void registerService(ServiceWrapper* servicewrapper);
	void unregisterService(ServiceWrapper* servicewrapper);
	void dispatchMessage(MessageContext* context);//分发客户端请求信息？
	ServiceWrapper* getService(const std::string& name) const;

private:
	class exec_func//嵌套类
	{
	public:
		std::string getServiceLocation();
		void parseResource();
		ResourcePath& getResourcePath();
		void deleteResourcePath();
		DeployedService* findServiceByPath();
	private:
		std::unordered_map<std::string, DeployedService> deployedservices;
		ResourcePath root;
	};
	exec_func* exec_func_ptr;//需要定义成指针常量

};

}

#endif