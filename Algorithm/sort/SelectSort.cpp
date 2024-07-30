/* ************************************************************************
> File Name:     SelectSort.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 10:35:47 AM CST
> Description:   
 ************************************************************************/

#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

// 平均    最坏  最好   空间   是否稳定
// O(n²)	O(n²)	O(n²)	O(1)	不稳定
void selectSort(vector<int>& elements)
{
  int maxIndex = 0;
  int len = elements.size();
  for (int i = 0; i < len; i++)
  {
    maxIndex = i;
    for (int j = i + 1; j < len; j++)
    {
      if (elements[maxIndex] < elements[j])
      {
        maxIndex = j;
      }
    }
    if (maxIndex != i)
    {
      swap(elements[maxIndex], elements[i]);
    }
  }
}

int main()
{
  vector<int> elements = {
      1, 4, 7, 2, 5, 8
  };
  selectSort(elements);
  for(const auto& element : elements) {
      cout<<element<<" ";
  }
  cout<<endl;
}