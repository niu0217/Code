/* ************************************************************************
> File Name:     simple5.cpp
> Author:        niu0217
> Created Time:  Mon 10 Jun 2024 09:01:14 PM CST
> Description:   解析天气文件siteList.xml
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "tinyxml2.h"       // 三方库 操作xml文件
#include <xlsxwriter.h>     // 三方库 操作excel文件
#include <boost/locale.hpp> // 三方库 boost

using namespace std;
using namespace tinyxml2;

vector<string> fileContexts;   // 保存要写入到文件中的内容
vector<vector<string>> excelContexts; // 保存到excel中的内容

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

    vector<string> cur;
    cur.push_back(string(code));
    cur.push_back(string(nameEn));
    cur.push_back(string(nameFr));
    cur.push_back(string(provinceCode));
    excelContexts.push_back(cur);

    site = site->NextSiblingElement("site");
  }
}

void parseXMLFile(const char *fileName)
{
  XMLDocument xml;
  xml.LoadFile(fileName);

  const char *version = xml.FirstChild()->ToDeclaration()->Value();
  fileContexts.push_back(string(version) + string("\n"));
  vector<string> cur;
  cur.push_back("code");
  cur.push_back("nameEn");
  cur.push_back("nameFr");
  cur.push_back("provinceCode");
  excelContexts.push_back(cur);

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

void writeToExcel(const char *outfileName)
{
  lxw_workbook *workbook = workbook_new(outfileName);
  lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

  int rowSize = excelContexts.size();
  int colSize = excelContexts[0].size();
  for(int row = 0; row < rowSize; row++)
  {
    for(int col = 0; col < colSize; col++)
    {
      string toWrite = excelContexts[row][col];
      string toWrite_iso88591 = boost::locale::conv::to_utf<char>(toWrite, "ISO-8859-1");
      worksheet_write_string(worksheet, row, col, toWrite_iso88591.c_str(), NULL);
    }
  }

  workbook_close(workbook);
}

int main()
{
  const char *fileName = "siteList.xml";
  parseXMLFile(fileName);
  // printContexts();

  // 写到文件中
  const char *outfileName = "./out/output.txt";
  writeToFile(outfileName);

  // 写到excel中
  outfileName = "./out/output.xlsx";
  writeToExcel(outfileName);
}