CXX_FLAGS = -std=c++14 -g -o
LIBS = -pthread

all:
	g++ ${CXX_FLAGS} client client.cpp ${LIBS}
	g++ ${CXX_FLAGS} epoll epoll_server.cpp

clean:
	rm client epoll
