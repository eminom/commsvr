Library = -Llib/libuv/out/Debug/obj.target
Source = src/echo.cc
Protocols = pb/dir.pb.cc pb/error_code.pb.cc pb/data.pb.cc pb/world.pb.cc
IncludeFlag = -Ilib/libuv/include\
	-Ipb

all:
	g++ -g -c ${IncludeFlag} ${Source}
	g++ -g -c ${IncludeFlag} ${Protocols}
	g++ *.o  ${Library} -lrt -luv -lprotobuf -o echo

clean:
	rm -rf a.out
	rm -rf *.o
	rm -rf *.pb.cc 
	rm -rf *.pb.h
