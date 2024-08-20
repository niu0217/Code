# 编译器
CXX = g++

# 编译选项
CXXFLAGS = -g -Wall -Werror

# 源文件
SRCS = main.cpp Add.cpp Sub.cpp

# 目标文件
OBJS = $(SRCS:.cpp=.o)

# 程序名称
TARGET = main

# 默认目标
all: $(TARGET)

# 链接目标文件，生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) -g -o $@ $^

# 编译源文件为目标文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清除构建产生的文件
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean