
#Library = -Llib/libuv/out/Debug/obj.target
Library = -Llib/libuv/build/Release \
	-L/usr/local/lib \
	-L/Users/eminom/dev/boost/macosx/lib \
	-Llib/libuv/libs

#Source =  src/StreamBuffer.cc src/StreamState.cc \
#			src/ProtoDispatcher.cc src/ProtoHandle.cc \
#			src/client_proc_t.cc src/ServerModel.cc \

Source = src/StreamBuffer.cc \
	src/client_proc_t.cc \
	src/Inception.cc \
	src/HelloInception.cc \
	src/DirWalker.cc \
	src/RootExplorer.cc 

ServerDist = src/Server.cc src/Echo.cc

Protocols = pb/cs_dir.pb.cc pb/exceptiontype.pb.cc \
	pb/data.pb.cc pb/cs_world.pb.cc
	
IncludeFlag = -Ilib/libuv/include\
	-Ipb\
	-I/usr/local/include\
	-I/Users/eminom/dev/boost/macosx/include\
	-Ilib/cJSON\
	-Ilib/haywire/include

JsonSrc = lib/cJSON/cJSON.c

HaywireSrc = \
	lib/haywire/src/haywire/http_response_cache.c \
	lib/haywire/src/haywire/connection_dispatcher.c \
	lib/haywire/src/haywire/http_parser.c \
	lib/haywire/src/haywire/route_compare_method.c \
	lib/haywire/src/haywire/http_request.c \
	lib/haywire/src/haywire/configuration/ini.c \
	lib/haywire/src/haywire/configuration/configuration.c \
	lib/haywire/src/haywire/server_stats.c \
	lib/haywire/src/haywire/http_server.c \
	lib/haywire/src/haywire/connection_consumer.c \
	lib/haywire/src/haywire/hw_string.c \
	lib/haywire/src/haywire/http_server_impl.c \
	lib/haywire/src/haywire/http_response.c

all:
	gcc -g -c ${IncludeFlag} ${JsonSrc} 
	g++ -std=c++11 -g -c ${IncludeFlag} ${Source}
	gcc -g -c -Ilib/haywire/include ${HaywireSrc}

	# The old commsvr
	#g++ -g -c ${IncludeFlag} ${Protocols}
	#g++ -D_DIRECTORY_DIST -g -c ${IncludeFlag} ${ServerDist}
	#g++ *.o  ${Library} -luv -lprotobuf -lboost_system -o out/dir
	#g++ -g -c ${IncludeFlag} ${ServerDist}
	#g++ *.o  ${Library} -luv -lprotobuf -lboost_system -o out/world

	# The new commsvr
	#g++ -g -c ${IncludeFlag} ${Source}
	g++ -std=c++11 src/CommSvr.cc *.o ${Library} -luv -o haysvr

clean:
	rm -rf a.out
	rm -rf *.o
	rm -rf *.pb.cc 
	rm -rf *.pb.h
