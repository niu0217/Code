/* ************************************************************************
> File Name:     simple3.cpp
> Author:        niu0217
> Created Time:  Mon 10 Jun 2024 09:01:14 PM CST
> Description:
 ************************************************************************/

#include "tinyxml2.h"
#include <iostream>

using namespace std;
using namespace tinyxml2;

void dealItems(const XMLDocument &xml)
{
  const XMLElement *items = xml.FirstChildElement("PurchaseOrder")->FirstChildElement("Items");
  const XMLElement *item = items->FirstChildElement("Item");
  while (item)
  {
    const XMLElement *productName = item->FirstChildElement("ProductName");
    const XMLElement *quantity = item->FirstChildElement("Quantity");
    const XMLElement *usPrice = item->FirstChildElement("USPrice");
    const XMLElement *comment = item->FirstChildElement("Comment");
    const XMLElement *shipDate = item->FirstChildElement("ShipDate");

    cout << "PartNumber: " << item->Attribute("PartNumber") << endl;
    cout << "ProductName: " << productName->GetText() << endl;
    cout << "Quantity: " << quantity->GetText() << endl;
    cout << "USPrice: " << usPrice->GetText() << endl;
    if (comment)
    {
      cout << "Comment: " << comment->GetText() << endl;
    }
    if (shipDate)
    {
      cout << "ShipDate: " << shipDate->GetText() << endl;
    }
    printf("\n");

    item = item->NextSiblingElement("Item");
  }
}

void dealAddress(const XMLDocument &xml)
{
  const XMLElement *root = xml.FirstChildElement("PurchaseOrder");
  const XMLElement *address = root->FirstChildElement("Address");
  while (address)
  {
    const XMLElement *name = address->FirstChildElement("Name");
    const XMLElement *street = address->FirstChildElement("Street");
    const XMLElement *city = address->FirstChildElement("City");
    const XMLElement *state = address->FirstChildElement("State");
    const XMLElement *zip = address->FirstChildElement("Zip");
    const XMLElement *country = address->FirstChildElement("Country");

    cout << "Address Type: " << address->Attribute("Type") << endl;
    cout << "Name: " << name->GetText() << endl;
    cout << "Street: " << street->GetText() << endl;
    cout << "City: " << city->GetText() << endl;
    cout << "State: " << state->GetText() << endl;
    cout << "Zip: " << zip->GetText() << endl;
    cout << "Country: " << country->GetText() << endl;
    printf("\n");

    address = address->NextSiblingElement("Address");
  }
}

void dealPurchaseOrder(const XMLDocument &xml)
{
  const XMLElement *root = xml.FirstChildElement("PurchaseOrder");
  const char *purchaseOrderNumber = root->Attribute("PurchaseOrderNumber");
  const char *orderDate = root->Attribute("OrderDate");
  cout << "PurchaseOrderNumber: " << purchaseOrderNumber << endl;
  cout << "OrderDate: " << orderDate << endl;
  printf("\n");

  dealAddress(xml);

  const XMLElement *deliveryNotes = root->FirstChildElement("DeliveryNotes");
  cout << "DeliveryNotes: " << deliveryNotes->GetText() << endl;
  printf("\n");

  dealItems(xml);
}

void parseXMLFile(const char *fileName)
{
  XMLDocument xml;
  xml.LoadFile(fileName);

  const char *version = xml.FirstChild()->ToDeclaration()->Value();
  cout << "version: " << version << endl;
  printf("\n");

  dealPurchaseOrder(xml);
}

int main()
{
  const char *fileName = "PurchaseOrder.xml";
  parseXMLFile(fileName);
}
