
#Library = -Llib/libuv/out/Debug/obj.target
Library = -Llib/libuv/build/Release -L/usr/local/lib

Source =  src/StreamBuffer.cc src/StreamState.cc \
			src/ProtoDispatcher.cc src/ProtoHandle.cc \
			src/client_proc_t.cc src/ServerModel.cc \

ServerDist = src/Server.cc src/Echo.cc

Protocols = pb/cs_dir.pb.cc pb/exceptiontype.pb.cc \
	pb/data.pb.cc pb/cs_world.pb.cc
	
IncludeFlag = -Ilib/libuv/include\
	-Ipb\
	-I/usr/local/include\
	-Ilib/cJSON

JsonSrc = lib/cJSON/cJSON.c

all:
	gcc -g -c ${IncludeFlag} ${JsonSrc} 
	g++ -g -c ${IncludeFlag} ${Source}
	g++ -g -c ${IncludeFlag} ${Protocols}
	g++ -D_DIRECTORY_DIST -g -c ${IncludeFlag} ${ServerDist}
	g++ *.o  ${Library} -luv -lprotobuf -o out/dir
	g++ -g -c ${IncludeFlag} ${ServerDist}
	g++ *.o  ${Library} -luv -lprotobuf -o out/world


clean:
	rm -rf a.out
	rm -rf *.o
	rm -rf *.pb.cc 
	rm -rf *.pb.h
