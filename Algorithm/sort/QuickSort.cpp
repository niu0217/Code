/* ************************************************************************
> File Name:     QuickSort.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 11:25:39 AM CST
> Description:   
 ************************************************************************/

#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

// 平均      最坏  最好       空间     是否稳定
// O(nlogn)	O(n²)	O(nlogn)	O(logn)	不稳定
int partition(vector<int>& elements, int low, int high)
{
  int pivot = elements[low];
  while (low < high)
  {
    // 从右向左找，第一个小于 pivot 的位置
    while(low < high && elements[high] > pivot)
    {
      high--;
    }
    if (low < high)
    {
      elements[low++] = elements[high];
    }
    // 从左向右找，第一个大于 pivot 的位置
    while (low < high && elements[low] < pivot)
    {
      low++;
    }
    if (low < high)
    {
      elements[high--] = elements[low];
    }
  }
  elements[low] = pivot;
  return low;
}

void quickSort(vector<int>& elements, int low, int high)
{
  if (low < high)
  {
    int pivot = partition(elements, low, high);
    quickSort(elements, low, pivot);
    quickSort(elements, pivot + 1, high);
  }
}

int main()
{
  vector<int> elements = {
      1, 4, 7, 2, 5, 8
  };
  int elementsSize = elements.size();
  quickSort(elements, 0, elementsSize - 1);
  for(const auto& element : elements) {
      cout<<element<<" ";
  }
  cout<<endl;
}