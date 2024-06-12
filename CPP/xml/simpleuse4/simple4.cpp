/* ************************************************************************
> File Name:     simple4.cpp
> Author:        niu0217
> Created Time:  Mon 10 Jun 2024 09:01:14 PM CST
> Description:   将PurchaseOrder.xml解析结果输出到文件中
 ************************************************************************/

#include "tinyxml2.h"
#include <iostream>

using namespace std;
using namespace tinyxml2;

void dealItems(const XMLDocument &xml, FILE *textfile)
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

    fprintf(textfile, "%s %s\n", "PartNumber: ", item->Attribute("PartNumber"));
    fprintf(textfile, "%s %s\n", "ProductName: ", productName->GetText());
    fprintf(textfile, "%s %s\n", "Quantity: ", quantity->GetText());
    fprintf(textfile, "%s %s\n", "USPrice: ", usPrice->GetText());
    if(comment)
    {
      fprintf(textfile, "%s %s\n", "Comment: ", comment->GetText());
    }
    if(shipDate)
    {
      fprintf(textfile, "%s %s\n", "ShipDate: ", shipDate->GetText());
    }

    item = item->NextSiblingElement("Item");
  }
}

void dealAddress(const XMLDocument &xml, FILE *textfile)
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

    fprintf(textfile, "%s %s\n", "Address Type: ", address->Attribute("Type"));
    fprintf(textfile, "%s %s\n", "Name: ", name->GetText());
    fprintf(textfile, "%s %s\n", "Street: ", street->GetText());
    fprintf(textfile, "%s %s\n", "City: ", city->GetText());
    fprintf(textfile, "%s %s\n", "State: ", state->GetText());
    fprintf(textfile, "%s %s\n", "Zip: ", zip->GetText());
    fprintf(textfile, "%s %s\n\n", "Country: ", country->GetText());

    address = address->NextSiblingElement("Address");
  }
}

void dealPurchaseOrder(const XMLDocument &xml, FILE *textfile)
{
  const XMLElement *root = xml.FirstChildElement("PurchaseOrder");
  const char *purchaseOrderNumber = root->Attribute("PurchaseOrderNumber");
  const char *orderDate = root->Attribute("OrderDate");
  fprintf(textfile, "%s %s\n%s %s\n\n", "PurchaseOrderNumber: ", purchaseOrderNumber,
          "OrderDate: ", orderDate);

  dealAddress(xml, textfile);

  const XMLElement *deliveryNotes = root->FirstChildElement("DeliveryNotes");
  fprintf(textfile, "%s %s\n\n", "DeliveryNotes: ", deliveryNotes->GetText());

  dealItems(xml, textfile);
}

void parseXMLFile(const char *fileName)
{
  XMLDocument xml;
  xml.LoadFile(fileName);

  // 设置输出文件
  const char* textFilePath = "./out/textfile.txt";
  FILE *textfile = fopen(textFilePath, "w");
  if(!textfile)
  {
    printf("打开文件 %s  失败\n", textFilePath);
    return;
  }

  const char *version = xml.FirstChild()->ToDeclaration()->Value();
  fprintf(textfile, "%s %s\n\n", "version: ", version);

  dealPurchaseOrder(xml, textfile);

  fclose(textfile);
}

int main()
{
  const char *fileName = "PurchaseOrder.xml";
  parseXMLFile(fileName);
}
