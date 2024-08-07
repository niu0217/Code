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

void merge(vector<int> &arr, int idxStart, int idxMid, int idxEnd)
{
  vector<int> leftArr(arr.begin() + idxStart,
                      arr.begin() + idxMid + 1);
  vector<int> rightArr(arr.begin() + idxMid + 1,
                       arr.begin() + idxEnd + 1);
  leftArr.insert(leftArr.end(), numeric_limits<int>::max());
  rightArr.insert(rightArr.end(), numeric_limits<int>::max());
  int left = 0;
  int right = 0;
  for (int i = idxStart; i <= idxEnd; i++)
  {
    if (leftArr[left] <rightArr[right])
    {
      arr[i] = leftArr[left++];
    }
    else
    {
      arr[i] = rightArr[right++];
    }
  }
}

void mergeSort(vector<int> &arr, int idxStart, int idxEnd)
{
  if (idxStart >= idxEnd)
  {
    return;
  }
  int idxMid = idxStart + (idxEnd - idxStart) / 2;
  mergeSort(arr, idxStart, idxMid);
  mergeSort(arr, idxMid + 1, idxEnd);
  merge(arr, idxStart, idxMid, idxEnd);
}

int main()
{
  vector<int> arr = { 100, 23, 45, 10000, 34, 99999, 34 };
  mergeSort(arr, 0, arr.size() - 1);
  for (const auto & num : arr)
  {
    cout << num << " ";
  }
  cout << endl;
}