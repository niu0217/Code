/* ************************************************************************
> File Name:     BubbleSort.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 10:32:26 AM CST
> Description:   
 ************************************************************************/


#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

// 平均    最坏  最好   空间   是否稳定
// O(n²)	O(n²)	O(n)	O(1)	稳定
void bubbleSort(vector<int>& elements)
{
  int len = elements.size();
  bool isSwaped = false;
  for (int i = 0; i < len; i++)
  {
    for (int j = 1; j < len - i; j++)
    {
      if (elements[j - 1] > elements[j])
      {
        swap(elements[j], elements[j - 1]);
        isSwaped = true;
      }
    }
    if (!isSwaped)
    {
      break;
    }
  }
}

int main()
{
  vector<int> elements = {
      1, 4, 7, 2, 5, 8
  };
  bubbleSort(elements);
  for(const auto& element : elements) {
      cout<<element<<" ";
  }
  cout<<endl;
}