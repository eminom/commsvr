
Library = -Llib/libuv/out/Debug/obj.target

Source =  src/StreamBuffer.cc src/StreamState.cc \
			src/ProtoDispatcher.cc src/ProtoHandle.cc \
			src/client_proc_t.cc src/ServerModel.cc

ServerDist = src/Server.cc src/Echo.cc

Protocols = pb/dir.pb.cc pb/error_code.pb.cc pb/data.pb.cc pb/world.pb.cc
IncludeFlag = -Ilib/libuv/include\
	-Ipb

all:
	g++ -g -c ${IncludeFlag} ${Source}
	g++ -g -c ${IncludeFlag} ${Protocols}
	g++ -D_DIRECTORY_DIST -g -c ${IncludeFlag} ${ServerDist}
	g++ *.o  ${Library} -lrt -luv -lprotobuf -o dir
	g++ -g -c ${IncludeFlag} ${ServerDist}
	g++ *.o  ${Library} -lrt -luv -lprotobuf -o world


clean:
	rm -rf a.out
	rm -rf *.o
	rm -rf *.pb.cc 
	rm -rf *.pb.h
	rm echo dir world
