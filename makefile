Library = -Llib/libuv/out/Debug/obj.target
Source = echo.c
Protocols = pb/dir.pb.cc pb/error_code.pb.cc pb/data.pb.cc pb/world.pb.cc
IncludeFlag = -Ilib/libuv/include\
	-Ipb

all:
	g++ -c ${IncludeFlag} ${Source}
	g++ -c ${IncludeFlag} ${Protocols}
	g++ *.o  ${Library} -lrt -luv -lprotobuf

clean:
	rm -rf a.out
	rm -rf *.o
	rm -rf *.pb.cc 
	rm -rf *.pb.h
