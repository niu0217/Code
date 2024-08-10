/* ************************************************************************
> File Name:     test.cpp
> Author:        niu0217
> Created Time:  Tue 30 Jul 2024 07:59:02 AM CST
> Description:   
 ************************************************************************/

// g++ -g -o test test.cpp

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Solution {
public:
    void getNext(string &s, vector<int> &next) {
        int j = 0;
        next[0] = 0;
        for (int i = 1; i < s.size(); i++) {
            while (j > 0 && s[i] != s[j]) {
                j = next[j - 1];
            }
            if (s[i] == s[j]) {
                j++;
            }
            next[i] = j;
        }
    }
public:
    int strStr(string haystack, string needle) {
        vector<int> next(haystack.size(), 0);
        getNext(haystack, next);
        for (int num : next) {
          cout << num << " ";
        }
        cout << endl;
        int j = 0;
        for (int i = 0; i < haystack.size(); i++) {
            while (j > 0 && haystack[i] != needle[j]) {
                j = next[j - 1];
            }
            if (haystack[i] == needle[j]) {
                j++;
            }
            if (j == needle.size()) {
                return i - needle.size() + 1;
            }
        }
        return -1;
    }
};

int main()
{
  Solution so;
  string s1;
  string s2;
  cin >> s1;
  cin >> s2;
  cout << so.strStr(s1, s2);
}