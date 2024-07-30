/* ************************************************************************
> File Name:     queue.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 04:32:30 PM CST
> Description:   
 ************************************************************************/

#include<iostream>
#include<vector>
#include<stdexcept>

class Queue {
public:
    bool empty() const {
        return elements.empty();
    }
    size_t size() const {
        return elements.size();
    }
    void push(int element) {
        elements.push_back(element);
    }
    int pop() {
        if(empty()) {
            throw std::out_of_range("Queue is empty, cannot dequeue!");
        }
        int front = elements.front();
        elements.erase(elements.begin());
        return front;
    }
    int front() const {
        if(empty()) {
            throw std::out_of_range("Queue is empty, cannot dequeue!");
        }
        return elements.front();
    }
private:
    std::vector<int> elements;
};

int main()
{
    Queue queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    queue.push(4);
    queue.push(5);
    queue.push(6);
    printf("%d\n", queue.pop());
    printf("%d\n", queue.front());
}