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

int partition(vector<int>& arr, int left, int right)
{
  int pivot = arr[left];
  while (left < right)
  {
    while (left < right && arr[right] >= pivot)
    {
      right--;
    }
    if (left < right)
    {
      arr[left++] = arr[right];
    }
    while (left < right && arr[left] <= pivot)
    {
      left++;
    }
    if (left < right)
    {
      arr[right--] = arr[left];
    }
  }
  arr[left] = pivot;
  return left;
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
  vector<int> arr = { 1, 4, 7, 2, 5, 8, 19, 30};
  quickSort(arr, 0, arr.size() - 1);
  for (const auto &num : arr)
  {
    cout << num << " ";
  }
  cout << endl;
}