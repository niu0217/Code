#include <tinyxml2.h>
#include <iostream>

using namespace std;
using namespace tinyxml2;
int main()
{
  XMLDocument xml;
  xml.LoadFile("test.xml");

  XMLElement *category = xml.RootElement();
  XMLElement *book = category->FirstChildElement("book");
  XMLElement *number = category->FirstChildElement("number");
  XMLElement *animal = category->FirstChildElement("animal");
  const char *book_name = book->GetText();
  cout << book_name << endl;
  int num = atoi(number->GetText());
  cout << num << endl;
  const char *animal_name = animal->GetText();
  cout << animal_name << endl;

  return 0;
}