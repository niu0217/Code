CXX_FLAGS = -std=c++14 -g -o
LIBS = -pthread

all:
	g++ ${CXX_FLAGS} client client.cpp ${LIBS}
	g++ ${CXX_FLAGS} select select_server.cpp

clean:
	rm client select
