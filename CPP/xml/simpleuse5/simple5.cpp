/* ************************************************************************
> File Name:     simple5.cpp
> Author:        niu0217
> Created Time:  Mon 10 Jun 2024 09:01:14 PM CST
> Description:   解析天气文件siteList.xml
 ************************************************************************/

#include "tinyxml2.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
using namespace tinyxml2;

vector<string> fileContexts;  // 保存要写入到文件中的内容

void parseSiteList(const XMLDocument &xml)
{
  const XMLElement *root = xml.FirstChildElement("siteList");
  const XMLElement *site = root->FirstChildElement("site");
  while(site)
  {
    const char *code = site->Attribute("code");
    const char *nameEn = site->FirstChildElement("nameEn")->GetText();
    const char *nameFr = site->FirstChildElement("nameFr")->GetText();
    const char *provinceCode = site->FirstChildElement("provinceCode")->GetText();

    fileContexts.push_back(string("code: ") + string(code));
    fileContexts.push_back(string("en: ") + string(nameEn));
    fileContexts.push_back(string("fr: ") + string(nameFr));
    fileContexts.push_back(string("provinceCode: ") + string(provinceCode) + string("\n"));

    site = site->NextSiblingElement("site");
  }
}

void parseXMLFile(const char *fileName)
{
  XMLDocument xml;
  xml.LoadFile(fileName);

  const char *version = xml.FirstChild()->ToDeclaration()->Value();
  fileContexts.push_back(string(version) + string("\n"));

  parseSiteList(xml);
}

void printContexts()
{
  for(string s : fileContexts)
  {
    cout<<s<<endl;
  }
}

void writeToFile(const char *outfileName)
{
  ofstream outfile(outfileName);
  if(!outfile.is_open())
  {
    cout<<"打开文件失败~~~~~~"<<endl;
    return;
  }
  for(string &s : fileContexts)
  {
    outfile<<s<<endl;
  }
  outfile.close();
}

int main()
{
  const char *fileName = "siteList.xml";
  parseXMLFile(fileName);
  // printContexts();

  // 写到文件中
  const char *outfileName = "./out/output.txt";
  writeToFile(outfileName);
}