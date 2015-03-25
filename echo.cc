
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include <string>
#include <cassert>

struct StreamBuffer
{
	StreamBuffer()
	{
		int sizePrefer = 2;
		begin_ptr_ = (char*)malloc(sizePrefer * sizeof(char));
		end_ptr_  = begin_ptr_ + sizePrefer;
		read_ptr_ = begin_ptr_;
		write_ptr_= begin_ptr_;
	}

	void append(char *start, int length)
	{
		printf("Start = %p,  length = %d\n", start, length);
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
			printf("Copy one\n");
			char *new_write_ptr = write_ptr_ + length;
			memcpy(write_ptr_, start, length * sizeof(char));
			write_ptr_ = new_write_ptr;
		}
		else if(totLeft >= length )
		{
			printf("Copy two\n");
			int offset = read_ptr_ - begin_ptr_;
			memmove(begin_ptr_, read_ptr_, offset * sizeof(char));
			read_ptr_ -= offset;
			write_ptr_ -= offset;
			append(start, length);  /// Recursive
		}
		else
		{
			printf("Copy three\n");
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

	bool readInt32(int &value)
	{
		if(payLength() < 4){
			return false;
		}
		int v = 0, b = 1;
		for(int i=0;i<4;++i){
			v += read_ptr_[i] * b;
			b <<= 4;
		}
		value = v;
		commit(4);
		return true;
	}

	bool readInt16(int &value)
	{
		if(payLength() < 2){
			return false;
		}
		int v = 0, b = 1;
		for(int i=0;i<2;++i){
			v += read_ptr_[i] * b;
			b <<= 4;
		}
		value = v;
		commit(2);
		return true;
	}

	bool readString(std::string &str, int length)
	{
		if(payLength()<length){
			return false;
		}
		str.resize(length);
		memcpy((char*)str.data(), read_ptr_, length * sizeof(char));
		commit(length);
		return true;
	}

	void commit(int offset)
	{
		read_ptr_ += offset;
	}

	int payLength()const{
		return write_ptr_ - read_ptr_;
	}

	char *read_ptr_;
	char *write_ptr_;
	char *begin_ptr_;
	char *end_ptr_;
};

StreamBuffer gStreamBuffer;

struct StreamState{
	enum State
	{
		Unknown,
		ZeroRead,
		LengthRead,
		TypeCodeRead,
		FullBodyRead,
	};
};

struct StreamStateObj
{
	StreamStateObj(StreamBuffer*buffer)
		:buffer_(buffer)
	{
		state_ = StreamState::ZeroRead;
	}

	//Changed
	void consume()
	{
		bool cont = true;
		while(cont)
		{
			cont = false;
			int length = 0, typecode = -1;
			std::string str;
			StreamState::State nextState = state_; // Keep
			switch(state_){
			case StreamState::ZeroRead:
				if(buffer_->readInt32(length)){
					full_length_ = length;
					nextState = StreamState::LengthRead;
					printf("Full length expecting: %d\n", full_length_);
					//printf("pay load length is %d\n", buffer_->payLength());
					cont = true;
				}
				break;
			case StreamState::LengthRead:
				if(buffer_->readInt16(typecode)){
					typecode_ = typecode;
					nextState = StreamState::TypeCodeRead;
					printf("Type code is: %d\n", typecode_);
					//printf("payload length is %d\n", buffer_->payLength());
					cont = true;
				}
				break;
			case StreamState::TypeCodeRead:
				if(buffer_->readString(str, full_length_ - 6)){
					payload_ = str;
					//ok, time to deal with payload
					printf("Consumed\n");
  				nextState = StreamState::ZeroRead;
					cont = true;
				}
				break;
			default:
				fprintf(stderr, "interesting ...%d>>\n", state_);
				abort();
				break;
			}
			state_ = nextState;
		}
	}	// end of consume

	StreamBuffer *buffer_;
	StreamState::State state_;
	int full_length_;
	int typecode_;
	std::string payload_;
};

StreamStateObj gStateObj(&gStreamBuffer);

#define DEFAULT_PORT 11000
#define DEFAULT_BACKLOG 128

uv_loop_t *loop;
struct sockaddr_in addr;

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free(req);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, NULL);
    } else if (nread > 0) {
			  printf("Read for %d\n", nread);
				gStreamBuffer.append(buf->base, nread);
				gStateObj.consume();
        //uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
        //uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
        //uv_write(req, client, &wrbuf, 1, echo_write);
    }

    if (buf->base)
        free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status) {
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
				printf("New connection made\n");
        uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
				printf("Starting read\n");
    }
    else {
        uv_close((uv_handle_t*) client, NULL);
    }
}

int main() {
    loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return 1;
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}
