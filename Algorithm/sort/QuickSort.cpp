/* ************************************************************************
> File Name:     QuickSort.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 11:25:39 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int partition(vector<int>& arr, int low, int high)
{
  int pivot = arr[low];
  while (low < high)
  {
    while (low < high && arr[high] >= pivot)
    {
      high--;
    }
    if (low < high)
    {
      arr[low++] = arr[high];
    }
    while (low < high && arr[low] <= pivot)
    {
      low++;
    }
    if (low < high)
    {
      arr[high--] = arr[low];
    }
  }
  arr[low] = pivot;
  return low;
}

void quickSort(vector<int>& arr, int left, int right)
{
  if (left < right)
  {
    int pivot = partition(arr, left, right);
    quickSort(arr, left, pivot - 1);
    quickSort(arr, pivot + 1, right);
  }
}

int main()
{
  vector<int> arr = { 1, 4, 7, 2, 5, 8, 90 };
  quickSort(arr, 0, arr.size() - 1);
  for (const auto &num : arr)
  {
    cout << num << " ";
  }
  cout << endl;
}