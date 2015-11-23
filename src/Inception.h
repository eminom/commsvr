

#ifndef _INCEPTION_LOOP_DEF_
#define _INCEPTION_LOOP_DEF_

#include <functional>
#include <string>

typedef std::function<void(const std::string)> InceptionCallback;

namespace inception
{
	int masterLoop(const InceptionCallback&);
	std::string retrieveLocalIP();
}

#endif