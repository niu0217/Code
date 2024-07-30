/* ************************************************************************
> File Name:     stack.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 04:31:34 PM CST
> Description:   
 ************************************************************************/

#include<iostream>
#include<vector>
#include<stdexcept>

class Stack {
public:
    void push(int element);
    void pop();
    int top() const;
    bool isEmpty() const {
        return elements.empty();
    }
private:
    std::vector<int> elements;  //使用vector作为实现栈的底层容器
};

void Stack::push(int element) {
    elements.emplace_back(element);
}

void Stack::pop() {
    if(isEmpty()) {
        throw std::out_of_range("Stack<>::pop():empty Stack");
    }
    elements.pop_back();
}

int Stack::top() const {
    if(isEmpty()) {
        throw std::out_of_range("Stack<>::pop():empty Stack");
    }
    return elements.back();
}

int main()
{
    try {
        Stack stack;
        stack.push(7);
        std::cout << stack.top() << std::endl; // 输出：7
        stack.pop();
        std::cout << stack.isEmpty() << std::endl; // 输出：1（true）
        stack.top(); // 将会抛出std::out_of_range异常，因为栈是空的
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl; // 输出异常信息
        return -1; // 返回非零值表示程序异常终止
    }
    return 0; // 程序正常退出，返回0值
}