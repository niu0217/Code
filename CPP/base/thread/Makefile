CFLAGS = -g -Wall -std=c++11
LIBS =  -lpthread

SRCS = thread_test1.cpp
EXECUTABLE = thread_test1
CC = g++

all:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRCS) $(LIBS)

.PHONY:clean
clean:
	rm $(EXECUTABLE)
