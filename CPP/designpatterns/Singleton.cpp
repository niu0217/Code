/* ************************************************************************
> File Name:     Singleton.cpp
> Author:        niu0217
> Created Time:  Mon 19 Aug 2024 09:52:16 AM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

class ShoppingCartManager
{
public:
  static ShoppingCartManager& getInstance()
  {
    static ShoppingCartManager instance;
    return instance;
  }

  void addCart(string itemName, int itemCnt)
  {
    if (m_cart.find(itemName) == m_cart.end())
    {
      m_order.emplace_back(itemName);
    }
    m_cart[itemName] += itemCnt;
  }

  void showCart()
  {
    for (const string &itemName : m_order)
    {
      cout << itemName << " " << m_cart[itemName] << endl;
    }
  }

private:
  ShoppingCartManager()
  {
  }

private:
  unordered_map<string, int> m_cart;
  vector<string> m_order;  // 保持输出的顺序
};

int main()
{
  string itemName;
  int itemCnt;
  
  while (cin >> itemName >> itemCnt)
  {
    ShoppingCartManager::getInstance().addCart(itemName, itemCnt);
  }

  ShoppingCartManager::getInstance().showCart();
}