/* ************************************************************************
> File Name:     array.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 04:28:21 PM CST
> Description:   
 ************************************************************************/

#include <cstddef>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>

template <typename T, std::size_t N>
class MyArray {
public:
    // 类型定义
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // 构造函数
    constexpr MyArray() noexcept { }
    explicit MyArray(const T& value) { fill(value); }
    MyArray(std::initializer_list<T> init) {
        if (init.size() > N) throw std::length_error("Initializer list too long");
        std::copy_n(init.begin(), init.size(), begin());
        if (init.size() < N) std::fill(begin() + init.size(), end(), T());
    }

    // 拷贝构造函数、拷贝赋值运算符、移动构造函数和移动赋值运算符可以默认
    MyArray(const MyArray&) = default;
    MyArray& operator=(const MyArray&) = default;
    MyArray(MyArray&&) = default; // 添加移动构造函数
    MyArray& operator=(MyArray&&) = default; // 添加移动赋值运算符

    // 访问元素
    reference at(size_type pos) {
        if (pos >= size()) throw std::out_of_range("Index out of range");
        return data[pos];
    }
    const_reference at(size_type pos) const {
        if (pos >= size()) throw std::out_of_range("Index out of range");
        return data[pos];
    }
    reference operator[](size_type pos) { return data[pos]; }
    const_reference operator[](size_type pos) const { return data[pos]; }
    reference front() { return data[0]; }
    const_reference front() const { return data[0]; }
    reference back() { return data[N - 1]; }
    const_reference back() const { return data[N - 1]; }

    // 迭代器相关
    iterator begin() noexcept { return data; }
    const_iterator begin() const noexcept { return data; }
    iterator end() noexcept { return data + N; }
    const_iterator end() const noexcept { return data + N; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_iterator cbegin() const noexcept { return begin(); } // 添加const版本的begin和end
    const_iterator cend() const noexcept { return end(); }

    // 容量相关
    constexpr bool empty() const noexcept { return N == 0; }
    constexpr size_type size() const noexcept { return N; }
    constexpr size_type max_size() const noexcept { return N; } // 由于是固定大小，max_size就是N

    // 修改操作
    void fill(const T& value) { std::fill(begin(), end(), value); }
    void swap(MyArray& other) noexcept { std::swap_ranges(begin(), end(), other.begin()); }

    // 添加一些常用算法作为成员函数（可选）
    iterator find(const T& value) { return std::find(begin(), end(), value); }
    const_iterator find(const T& value) const { return std::find(cbegin(), cend(), value); }
    bool contains(const T& value) const { return find(value) != end(); } // C++20中std::array没有这个函数，但添加它很有用。
    size_t count(const T& value) const { return std::count(cbegin(), cend(), value); } // 计算某个值出现的次数。
    iterator erase(const_iterator pos) { // 注意：这个实现不支持删除多个元素，因为数组大小是固定的。仅用于模拟接口。
        if (pos == end()) throw std::out_of_range("Cannot erase end position");
        std::move(pos + 1, end(), pos); // 将后面的元素向前移动一个位置。注意这不是一个高效的操作。
        end()[-1] = T(); // 将最后一个元素设置为默认值。注意这可能会导致不必要的对象构造/析构。
        return pos; // 返回被删除元素的下一个位置。注意这不是标准std::array的erase行为。
    } // 在实际应用中，通常不会为固定大小的容器实现erase函数。这里只是为了展示如何模拟接口。
    // 注意：对于固定大小的数组，通常不会实现插入操作，因为会涉及到移动所有后续元素。这里没有提供insert函数。

private:
    T data[N]; // 固定大小的数组作为内部存储结构
};

// 示例用法：输出数组元素、查找、包含检查和修改数组元素等操作。
int main()
{
    MyArray<int, 5> arr{1, 2, 3, 4, 5}; // 使用初始化列表构造数组并初始化元素。
    for (int num : arr) std::cout << num << " "; // 输出: 1 2 3 4 5，然后换行。
    std::cout << '\n'; // 输出换行符以保持输出整洁。
    auto it = arr.find(3); // 查找元素3，并返回它的迭代器。这里会输出数组下标2对应的元素（如果找到的话）。注意下标是从0开始的。
    if (it != arr.end()) std::cout << "Found " << *it << " at index " << (it - arr.begin()) << '\n';
}