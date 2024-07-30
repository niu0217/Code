/* ************************************************************************
> File Name:     InsertSort.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 10:38:51 AM CST
> Description:   
 ************************************************************************/

#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

// 平均    最坏  最好   空间   是否稳定
// O(n²)	O(n²)	O(n)	O(1)	稳定
void insertSort(vector<int>& elements)
{
  int len = elements.size();
  for (int i = 1; i < len; i++)
  {
    for (int j = i; j > 0; j--)
    {
      if (elements[j - 1] > elements[j])
      {
        swap(elements[j - 1], elements[j]);
      }
      else
      {
        break;
      }
    }
  }
}

int main()
{
  vector<int> elements = {
      1, 4, 7, 2, 5, 8
  };
  insertSort(elements);
  for(const auto& element : elements) {
      cout<<element<<" ";
  }
  cout<<endl;
}