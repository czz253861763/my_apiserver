#ifndef ENGINE
#define ENGINE

#include "ServiceDispatcher.h"
#include "FilterDispatcher.h"

namespace apiserver {

class Engine
{
private:
	ServiceDispatcher& servicedispatcher;
	FilterDispatcher* filterdispather = nullptr;
};


}

#endif