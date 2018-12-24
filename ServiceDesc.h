#ifndef SERVICEDESC
#define SERVICEDESC

namespace apiserver {

//参数描述结构体
struct ParameterDescription
{
	//枚举类
	enum class Type
	{
		Unknown,
		Any
	};
	std::string name;
	Type type;
	bool nullable;
};

//操作描述结构体
struct OperationDescription
{
	std::string name;
	std::string location;
	int method;
	std::string methodstr;
	bool asy;
	std::string desc;
	std::string details;
	std::vector<ParameterDescription> parameters;
	ParameterDescription::Type res;
	bool resultNullable;
};

struct ServiceDescription
{
	std::string name;
	std::string location;
	std::string desc;
	std::string details;
	std::vector<OperationDescription> operations;//操作描述列表
};


}

#endif