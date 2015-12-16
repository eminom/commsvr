


#include "HttpServerConfig.h"

bool getFileMimeType(const std::string &suffix, std::string &mmType)
{

#ifdef _EE
#error Shall not be defined before
#else
#define _EE(n, m)	else if (#n == suffix){ mmType = (m); return true;}
#endif

#ifdef _EE_PLT
#error Shall not be defined before
#else
#define _EE_PLT(n)	_EE(n, "text/plain")
#endif

	if(0){}
	_EE(html, "text/html")
	_EE_PLT(txt)

	_EE_PLT(mk)
	_EE_PLT(gitignore)
	_EE_PLT(gitmodules)
	_EE_PLT(lua)
	_EE_PLT(js)
	_EE_PLT(py)
	_EE_PLT(pl)
	_EE_PLT(rb)
	_EE_PLT(php)
	_EE_PLT(c)
	_EE_PLT(h)
	_EE_PLT(cc)
	_EE_PLT(erl)
	_EE_PLT(sh)
	_EE_PLT(json)

	_EE_PLT(tlog)
	_EE_PLT(filters)
	_EE_PLT(vcxproj)
	_EE_PLT(user)

	_EE(xml,  "text/xml")

	_EE(bmp,  "application/x-bmp")
	_EE(jpe,  "application/x-jpe")

	_EE(png,  "image/png")
	_EE(ico,  "image/x-icon")
	_EE(fax,  "image/fax")
	_EE(gif,  "image/gif")
	_EE(net,  "image/pnetvue")
	_EE(rp,   "image/vnd.rn-realpix")
	_EE(wbmp, "image/vnd.wap.wbmp")
	_EE(jpg,  "image/jpeg")
	_EE(jpeg, "image/jpeg")
	_EE(jfif, "image/jpeg")
	_EE(tif,  "image/tiff")
	_EE(tiff, "image/tiff")

#undef _EE

	mmType = "application/octet-stream";
	return false;
}

