CXX = g++
CXXFLAGS = -std=c++11 -g
SRCS = boost_coroutine2_test.cpp
LIBS = -lboost_coroutine -lboost_context

.PHONY: all clean

TARGET = boost_coroutine2_test
TARGET += ucontext_test

all: ${TARGET}

boost_coroutine2_test: boost_coroutine2_test.cpp
	${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

ucontext_test: ucontext_test.cpp
	${CXX} ${CXXFLAGS} -o $@ $^ -lpthread

clean:
	rm -f ${TARGET}