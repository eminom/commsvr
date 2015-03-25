Library = -Llib/libuv/out/Debug/obj.target
Source = uvcat.c
IncludeFlag = -Ilib/libuv/include 

all:
	gcc ${Library} ${IncludeFlag} ${Source} -lrt -luv


clean:
	rm -rf *.o
