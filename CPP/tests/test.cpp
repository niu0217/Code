/* ************************************************************************
> File Name:     test.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 07:59:02 AM CST
> Description:   
 ************************************************************************/

// g++ -g -o test test.cpp

#include <iostream>
#include <typeinfo>

class Base {
public:
    virtual ~Base() {}  // 确保有虚析构函数以启用 RTTI
};

class Derived : public Base {
public:
    void show() {
        std::cout << "Derived class" << std::endl;
    }
};

class AnotherClass : public Base {
public:
    void display() {
        std::cout << "Another class" << std::endl;
    }
};

// 自定义 dynamic_cast 实现
template<typename TargetType, typename SourceType>
TargetType safe_dynamic_cast(SourceType* source) {
    // 使用 typeid 进行类型检查
    if (dynamic_cast<TargetType>(source) != nullptr) {
        return static_cast<TargetType>(source);
    }
    return nullptr;  // 转换失败时返回 nullptr
}

int main() {
    Base* base = new Derived();  // 创建派生类对象

    // 使用自定义动态转换
    Derived* derived = safe_dynamic_cast<Derived*>(base);
    if (derived) {
        derived->show();  // 输出: Derived class
    } else {
        std::cout << "Casting failed!" << std::endl;
    }

    AnotherClass* another = safe_dynamic_cast<AnotherClass*>(base);
    if (another) {
        another->display();  // 不会被调用
    } else {
        std::cout << "Casting failed!" << std::endl;  // 输出: Casting failed!
    }

    delete base;  // 清理内存
    return 0;
}