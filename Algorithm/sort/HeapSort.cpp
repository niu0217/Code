/* ************************************************************************
> File Name:     HeapSort.cpp
> Author:        niu0217
> Created Time:  Sat 03 Aug 2024 01:39:00 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void heapify(vector<int> &arr, int n, int i)
{
  int largest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;
  if (left < n && arr[left] > arr[largest])
  {
    largest = left;
  }
  if (right < n && arr[right] > arr[largest])
  {
    largest = right;
  }
  if (largest != i)
  {
    swap(arr[largest], arr[i]);
    heapify(arr, n, largest);
  }
}

void heapSort(vector<int> &arr)
{
  int n = arr.size();
  for (int i = (n / 2) - 1; i >= 0; i--)
  {
    heapify(arr, n, i);
  }
  for (int i = n - 1; i >= 0; i--)
  {
    swap(arr[i], arr[0]);
    heapify(arr, i, 0);
  }
}

int main()
{
  vector<int> arr = { 1, 4, 7, 2, 5, 8 };
  heapSort(arr);
  for (int num : arr)
  {
    cout << num << " ";
  }
  cout << endl;
}