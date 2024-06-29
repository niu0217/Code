CFLAGS = -g -Wall -std=c++11
LIBS =  -lpthread

SRCS = thread_sem_pc.cpp
EXECUTABLE = thread_sem_pc
CC = g++

all:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRCS) $(LIBS)

.PHONY:clean
clean:
	rm $(EXECUTABLE)
