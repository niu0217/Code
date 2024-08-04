/* ************************************************************************
> File Name:     MergeSort.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 11:02:21 AM CST
> Description:   
 ************************************************************************/

#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

// 平均      最坏       最好      空间   是否稳定
// O(nlogn)	O(nlogn)	O(nlogn)	O(n)	稳定
void merge(vector<int>& elements, int idxStart, int idxMid, int idxEnd)
{
  vector<int> leftArr(elements.begin() + idxStart,
                      elements.begin() + idxMid + 1);
  vector<int> rightArr(elements.begin() + idxMid + 1,
                       elements.begin() + idxEnd + 1);
  leftArr.insert(leftArr.end(), numeric_limits<int>::max());
  rightArr.insert(rightArr.end(), numeric_limits<int>::max());
  int left = 0;
  int right = 0;
  for (int i = idxStart; i <= idxEnd; i++)
  {
    if (leftArr[left] < rightArr[right])
    {
      elements[i] = leftArr[left++];
    }
    else
    {
      elements[i] = rightArr[right++];
    }
  }
}

void mergeSort(vector<int>& elements, int idxStart, int idxEnd)
{
  if (idxStart >= idxEnd)
  {
    return;
  }
  int idxMid = idxStart + (idxEnd - idxStart) / 2;
  mergeSort(elements, idxStart, idxMid);
  mergeSort(elements, idxMid + 1, idxEnd);
  merge(elements, idxStart, idxMid, idxEnd);
}

int main()
{
  vector<int> elements = {
      1, 4, 7, 2, 5, 8
  };
  int elementsSize = elements.size();
  mergeSort(elements, 0, elementsSize - 1);
  for(const auto& element : elements) {
      cout<<element<<" ";
  }
  cout<<endl;
}