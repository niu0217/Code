/* ************************************************************************
> File Name:     MergeSort.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 11:02:21 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void merge(vector<int> &arr, int left, int mid, int right)
{
  vector<int> leftArr(arr.begin() + left,
                      arr.begin() + mid + 1);
  vector<int> rightArr(arr.begin() + mid + 1,
                       arr.begin() + right + 1);
  leftArr.insert(leftArr.end(), numeric_limits<int>::max());
  rightArr.insert(rightArr.end(), numeric_limits<int>::max());
  int i = 0;
  int j = 0;
  for (int k = left; k <= right; k++)
  {
    if (leftArr[i] < rightArr[j])
    {
      arr[k] = leftArr[i++];
    }
    else
    {
      arr[k] = rightArr[j++];
    }
  }
}

void mergeSort(vector<int> &arr, int left, int right)
{
  if (left >= right)
  {
    return;
  }
  int mid = left + (right - left) / 2;
  mergeSort(arr, left, mid);
  mergeSort(arr, mid + 1, right);
  merge(arr, left, mid, right);
}

int main()
{
  vector<int> arr = { 1, 4, 7, 2, 5, 8 };
  mergeSort(arr, 0, arr.size() - 1);
  for (int num : arr)
  {
    cout << num << " ";
  }
  cout << endl;
}