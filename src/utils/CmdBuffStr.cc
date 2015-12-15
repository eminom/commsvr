
#include "CmdBuffStr.h"

static CmdStringBuffer *_cmd;
CmdStringBuffer* CmdStringBuffer::getInstance()
{
	if( ! _cmd )
	{
		_cmd = new CmdStringBuffer;
	}
	return _cmd;
}

CmdStringBuffer::~CmdStringBuffer()
{
	delete _buffer;
}

CmdStringBuffer::CmdStringBuffer()
	:_buffer(new StreamBuffer),
	_callback([](const std::string& msg){
		printf("Upload <%s>\n", msg.c_str());
	})
{
}

void CmdStringBuffer::imbibe(const char *start, int length)
{
	int cur = 0, pre = 0;
	while ( cur < length )
	{
		if('\n' == start[cur])
		{
			if (cur - pre - 1 > 0)
			{
				//~ - 1 removes the last \r(if you test with telnet)
				_buffer->append(start + pre, cur - pre - 1);
				std::string content;
				if( _buffer->readString(content) )
				{
					_callback(content);
				}
			}
			cur = cur + 1;
			pre = cur;
		}
		else
		{
			cur++;
		}
	}
	//Length of 0 is acceptable.
	_buffer->append(start + pre, cur - pre);
}

void CmdStringBuffer::setCallback(const std::function<void(const std::string&)>& cb)
{
	_callback = cb;
}