

#ifndef _STREAM_BUFFER_DEF__
#define _STREAM_BUFFER_DEF__

#include <string>

class StreamBuffer
{
public:
	StreamBuffer(const char *buffer, int length);
	StreamBuffer();
	virtual ~StreamBuffer();

public:
	void append(char *start, int length);
	bool readString(std::string &str, int length);
	
	int payLength()const;
	void commit(int offset);

protected:
	char *read_ptr_;
	char *write_ptr_;
	char *begin_ptr_;
	char *end_ptr_;
	bool owned_;

private:
	StreamBuffer(const StreamBuffer&);
	void operator=(const StreamBuffer&);
};



#endif