

#ifndef _INCEPTION_LOOP_DEF_
#define _INCEPTION_LOOP_DEF_

#include <functional>
#include <string>

typedef std::function<void(const std::string)> InceptionCallback;

namespace inception
{
	int masterLoop(const InceptionCallback&);
	int helloLoop(const char *serverRootDir);
    int getMonitorServerPort();

	std::string retrieveLocalIP();
}

#endif
