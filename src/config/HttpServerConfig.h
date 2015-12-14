

#ifndef _SERVER_CONF_FOR_COMMSVR_DEF__
#define _SERVER_CONF_FOR_COMMSVR_DEF__

#include <string>

bool isPlainTextSuffix(const std::string&);
bool isImageSuffix(const std::string&);
std::string getImageMimeType(const std::string &);


//~ 
#define _DefaultMonitorSvrPort  13000

#endif
