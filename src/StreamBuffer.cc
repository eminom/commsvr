
#include "StreamBuffer.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <cassert>

StreamBuffer::StreamBuffer():owned_(true)
{
	int sizePrefer = 2;
	begin_ptr_ = (char*)malloc(sizePrefer * sizeof(char));
	end_ptr_  = begin_ptr_ + sizePrefer;
	read_ptr_ = begin_ptr_;
	write_ptr_= begin_ptr_;
}

StreamBuffer::StreamBuffer(const char *start, int length)
	:owned_(false)
{
	begin_ptr_ = const_cast<char*>(start);
	end_ptr_ = begin_ptr_ + length;
	read_ptr_ = begin_ptr_;
	write_ptr_ = end_ptr_;  //Useless
}

StreamBuffer::~StreamBuffer()
{
	if(owned_)
	{
		free(begin_ptr_);
	}
}

void StreamBuffer::append(char *start, int length)
{
	if(!owned_)
	{
		return;
	}

	//printf("Start = %p,  length = %d\n", start, length);
	if(length <= 0 ){
		return;
	}
	//printf("Append +\n");
	int pay_length = write_ptr_ - read_ptr_;
	int left = end_ptr_ - write_ptr_;
	//printf("Left is %d\n", left);
	assert( read_ptr_ >= begin_ptr_);
	int totLeft = left + (read_ptr_ - begin_ptr_);
	if (left >= length)
	{
		//printf("Copy one\n");
		char *new_write_ptr = write_ptr_ + length;
		memcpy(write_ptr_, start, length * sizeof(char));
		write_ptr_ = new_write_ptr;
	}
	else if(totLeft >= length )
	{
		//printf("Copy two\n");
		int offset = read_ptr_ - begin_ptr_;
		memmove(begin_ptr_, read_ptr_, offset * sizeof(char));
		read_ptr_ -= offset;
		write_ptr_ -= offset;
		append(start, length);  /// Recursive
	}
	else
	{
		//printf("Copy three\n");
		int size = (end_ptr_ - begin_ptr_) * 2;
		char *new_begin_ptr = (char*)malloc(size * sizeof(char));
		char *new_end_ptr = new_begin_ptr + size;
		char *new_read_ptr = new_begin_ptr;
		char *new_write_ptr = new_begin_ptr + pay_length;
		memcpy(new_read_ptr, read_ptr_, pay_length * sizeof(char));

		begin_ptr_ = new_begin_ptr;
		end_ptr_   = new_end_ptr;
		read_ptr_  = new_read_ptr;
		write_ptr_ = new_write_ptr;
		append(start, length);
	}
	//printf("Append - \n");
}


bool StreamBuffer::readString(std::string &str, int length)
{
	if(payLength()<length){
		return false;
	}
	str.resize(length);
	memcpy((char*)str.data(), read_ptr_, length * sizeof(char));
	commit(length);
	return true;
}

void StreamBuffer::commit(int offset)
{
	read_ptr_ += offset;
}

int StreamBuffer::payLength()const{
	return write_ptr_ - read_ptr_;
}
