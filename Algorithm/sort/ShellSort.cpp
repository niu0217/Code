/* ************************************************************************
> File Name:     ShellSort.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 11:01:13 AM CST
> Description:   
 ************************************************************************/

#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

void shellSort(vector<int>& elements) {
    int gap = 1;
    int size = elements.size();
    while(gap < size / 3) {
        gap = 3 * gap + 1;
    }
    while(gap >= 1) {
        for(int i = gap; i < size; i++) {
            for(int j = i; j >= gap && elements[j] < elements[j - gap]; j -= gap) {
                swap(elements[j], elements[j - gap]);
            }
        }
        gap = gap / 3;
    }
}

int main()
{
    vector<int> elements = {
        1, 4, 7, 2, 5, 8
    };
    shellSort(elements);
    for(const auto& element : elements) {
        cout<<element<<" ";
    }
    cout<<endl;
}