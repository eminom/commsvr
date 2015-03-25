Library = -Llib/libuv/out/Debug/obj.target
Source = echo.c
IncludeFlag = -Ilib/libuv/include 

all:
	gcc ${Library} ${IncludeFlag} ${Source} -lrt -luv


clean:
	rm -rf *.o
	rm -rf *.pb.cc 
	rm -rf *.pb.h
