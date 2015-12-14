


#include "HttpServerConfig.h"

bool isPlainTextSuffix(const std::string &suffix)
{
	return "lua" == suffix 
		|| "tlog" == suffix 
		|| "txt" == suffix 
		|| "js" == suffix 
		|| "py" == suffix 
		|| "pl" == suffix
		|| "filters" == suffix
		|| "vcxproj" == suffix
		|| "user" == suffix;
}

bool isImageSuffix(const std::string &suffix)
{
	return "png" == suffix 
		|| "jpg" == suffix 
		|| "jpeg" == suffix;
}


std::string getImageMimeType(const std::string &suffix)
{
	if("png" == suffix){
		return "image/png";
	} else if( "jpg" == suffix || "jpeg" == suffix){
		return "image/jpeg";
	}
	return "image/png"; ///` The default
}

