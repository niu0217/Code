CFLAGS = -g -Wall -std=c++14
LIBS =  -lpthread

SRCS = version2_main.cpp
EXECUTABLE = main
CC = g++

all:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRCS) $(LIBS)

.PHONY:clean
clean:
	rm $(EXECUTABLE)
