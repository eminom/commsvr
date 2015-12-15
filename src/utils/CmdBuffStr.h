
#pragma once
#include "StreamBuffer.h"
#include <functional>

class CmdStringBuffer
{
private:
	CmdStringBuffer();
	~CmdStringBuffer();

public:
	static CmdStringBuffer* getInstance();
	void setCallback(const std::function<void(const std::string&)>&);
	void imbibe(const char *start, int length);

private:
	StreamBuffer *const _buffer;
	std::function<void(const std::string&)> _callback;

private:
	CmdStringBuffer(const CmdStringBuffer&);
	void operator=(const CmdStringBuffer&);
};
