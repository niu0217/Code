CFLAGS = -g -Wall -std=c++11
LIBS =  -lpthread

SRCS = threadlock_c11.cpp
EXECUTABLE = threadlock_c11
CC = g++

all:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRCS) $(LIBS)

.PHONY:clean
clean:
	rm $(EXECUTABLE)
