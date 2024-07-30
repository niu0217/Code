/* ************************************************************************
> File Name:     test.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 07:59:02 AM CST
> Description:   
 ************************************************************************/

// g++ -g -o test test.cpp

#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec;

    // 添加一些元素
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    std::cout << "Size: " << vec.size() << ", Capacity: " << vec.capacity() << std::endl;

    // 删除所有元素
    vec.clear();
    std::cout << "Size after clear: " << vec.size() << ", Capacity: " << vec.capacity() << std::endl;

    // 调整容量
    vec.shrink_to_fit();
    std::cout << "Size after shrink_to_fit: " << vec.size() << ", Capacity: " << vec.capacity() << std::endl;

    return 0;
}