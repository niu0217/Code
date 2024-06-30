CFLAGS = -g -Wall -std=c++14
LIBS =  -lpthread

SRCS = version3_main.cpp TaskQueue.cpp ThreadPool.cpp
EXECUTABLE = main
CC = g++

all:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRCS) $(LIBS)

.PHONY:clean
clean:
	rm $(EXECUTABLE)
