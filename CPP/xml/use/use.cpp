/* ************************************************************************
> File Name:     use.cpp
> Author:        niu0217
> Created Time:  Mon 10 Jun 2024 09:49:18 PM CST
> Description:
 ************************************************************************/

#if defined(_MSC_VER)
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS // This test file is not intended to be secure.
#endif
#endif

#include "tinyxml2.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

#if defined(_MSC_VER) || defined(WIN32)
#include <crtdbg.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
_CrtMemState startMemState;
_CrtMemState endMemState;
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

using namespace tinyxml2;
using namespace std;
int gPass = 0;
int gFail = 0;

bool XMLTest(const char *testString, const char *expected, const char *found, bool echo = true, bool extraNL = false)
{
  bool pass;
  if (!expected && !found)
    pass = true;
  else if (!expected || !found)
    pass = false;
  else
    pass = !strcmp(expected, found);
  if (pass)
    printf("[pass]");
  else
    printf("[fail]");

  if (!echo)
  {
    printf(" %s\n", testString);
  }
  else
  {
    if (extraNL)
    {
      printf(" %s\n", testString);
      printf("%s\n", expected);
      printf("%s\n", found);
    }
    else
    {
      printf(" %s [%s][%s]\n", testString, expected, found);
    }
  }

  if (pass)
    ++gPass;
  else
    ++gFail;
  return pass;
}

bool XMLTest(const char *testString, XMLError expected, XMLError found, bool echo = true, bool extraNL = false)
{
  return XMLTest(testString, XMLDocument::ErrorIDToName(expected), XMLDocument::ErrorIDToName(found), echo, extraNL);
}

bool XMLTest(const char *testString, bool expected, bool found, bool echo = true, bool extraNL = false)
{
  return XMLTest(testString, expected ? "true" : "false", found ? "true" : "false", echo, extraNL);
}

template <class T>
bool XMLTest(const char *testString, T expected, T found, bool echo = true)
{
  bool pass = (expected == found);
  if (pass)
    printf("[pass]");
  else
    printf("[fail]");

  if (!echo)
    printf(" %s\n", testString);
  else
  {
    char expectedAsString[64];
    XMLUtil::ToStr(expected, expectedAsString, sizeof(expectedAsString));

    char foundAsString[64];
    XMLUtil::ToStr(found, foundAsString, sizeof(foundAsString));

    printf(" %s [%s][%s]\n", testString, expectedAsString, foundAsString);
  }

  if (pass)
    ++gPass;
  else
    ++gFail;
  return pass;
}

void NullLineEndings(char *p)
{
  while (p && *p)
  {
    if (*p == '\n' || *p == '\r')
    {
      *p = 0;
      return;
    }
    ++p;
  }
}

void test1()
{
  static const char *test[] = {"<element />",
                               "<element></element>",
                               "<element><subelement/></element>",
                               "<element><subelement></subelement></element>",
                               "<element><subelement><subsub/></subelement></element>",
                               "<!--comment beside elements--><element><subelement></subelement></element>",
                               "<!--comment beside elements, this time with spaces-->  \n <element>  <subelement> \n </subelement> </element>",
                               "<element attrib1='foo' attrib2=\"bar\" ></element>",
                               "<element attrib1='foo' attrib2=\"bar\" ><subelement attrib3='yeehaa' /></element>",
                               "<element>Text inside element.</element>",
                               "<element><b></b></element>",
                               "<element>Text inside and <b>bolded</b> in the element.</element>",
                               "<outer><element>Text inside and <b>bolded</b> in the element.</element></outer>",
                               "<element>This &amp; That.</element>",
                               "<element attrib='This&lt;That' />",
                               0};
  for (int i = 0; test[i]; ++i)
  {
    XMLDocument doc;
    doc.Parse(test[i]);
    doc.Print();
    printf("----------------------------------------------\n");
  }
}

void test2()
{
  static const char *test = "<!--hello world\n"
                            "          line 2\r"
                            "          line 3\r\n"
                            "          line 4\n\r"
                            "          line 5\r-->";

  XMLDocument doc;
  doc.Parse(test);
  doc.Print();
}

void test3()
{
  // This test is pre-test for the next one
  // (where Element1 is inserted "after itself".
  // This code didn't use to crash.
  XMLDocument doc;
  XMLElement *element1 = doc.NewElement("Element1");
  XMLElement *element2 = doc.NewElement("Element2");
  doc.InsertEndChild(element1);
  doc.InsertEndChild(element2);
  doc.Print();
  cout << "----------------\n";
  doc.InsertAfterChild(element2, element2);
  doc.InsertAfterChild(element2, element2);
  doc.Print();
  cout << "----------------\n";
}

void test4()
{
  XMLDocument doc;
  XMLElement *element1 = doc.NewElement("Element1");
  XMLElement *element2 = doc.NewElement("Element2");
  doc.InsertEndChild(element1);
  doc.InsertEndChild(element2);
  doc.Print();
  cout << "----------------\n\n";

  // This insertion "after itself"
  // used to cause invalid memory access and crash
  doc.InsertAfterChild(element1, element1);
  doc.InsertAfterChild(element1, element1);
  doc.InsertAfterChild(element2, element2);
  doc.InsertAfterChild(element2, element2);
  doc.Print();
  cout << "----------------\n\n";
}

void test5()
{
  static const char *test = "<element>Text before.</element>";
  XMLDocument doc;
  doc.Parse(test);
  XMLElement *root = doc.FirstChildElement();
  XMLElement *newElement = doc.NewElement("Subelement");
  root->InsertEndChild(newElement);
  doc.Print();
}

void test6()
{
  XMLDocument *doc = new XMLDocument();
  static const char *test = "<element><sub/></element>";
  doc->Parse(test);
  delete doc;
}

void test7()
{
  // Test: Programmatic DOM nodes insertion return values
  XMLDocument doc;

  XMLNode *first = doc.NewElement("firstElement");
  XMLNode *firstAfterInsertion = doc.InsertFirstChild(first);
  doc.Print();
  cout << "----------------\n\n";

  XMLNode *last = doc.NewElement("lastElement");
  XMLNode *lastAfterInsertion = doc.InsertEndChild(last);
  doc.Print();
  cout << "----------------\n\n";

  XMLNode *middle = doc.NewElement("middleElement");
  XMLNode *middleAfterInsertion = doc.InsertAfterChild(first, middle);
  doc.Print();
  cout << "----------------\n\n";
}

void test8()
{
  // Test: Programmatic DOM
  // Build:
  //		<element>
  //			<!--comment-->
  //			<sub attrib="0" />
  //			<sub attrib="1" />
  //			<sub attrib="2" >& Text!</sub>
  //		<element>

  XMLDocument *doc = new XMLDocument();
  XMLNode *element = doc->InsertEndChild(doc->NewElement("element"));

  XMLElement *sub[3] = {doc->NewElement("sub"), doc->NewElement("sub"), doc->NewElement("sub")};
  for (int i = 0; i < 3; ++i)
  {
    sub[i]->SetAttribute("attrib", i);
  }
  // <element/>
  doc->Print();
  cout << "\n\n";

  element->InsertEndChild(sub[2]);

  /*
    <element>
      <sub attrib="2"/>
    </element>
  */
  doc->Print();
  cout << "\n\n";

  const int dummyInitialValue = 1000;
  int dummyValue = dummyInitialValue;
  XMLNode *comment = element->InsertFirstChild(doc->NewComment("comment"));
  comment->SetUserData(&dummyValue);
  element->InsertAfterChild(comment, sub[0]);
  element->InsertAfterChild(sub[0], sub[1]);
  sub[2]->InsertFirstChild(doc->NewText("& Text!"));
  /*
    <element>
      <!--comment-->
      <sub attrib="0"/>
      <sub attrib="1"/>
      <sub attrib="2">&amp; Text!</sub>
    </element>
  */
  doc->Print();
  cout << "\n\n";

  // // And now deletion:
  element->DeleteChild(sub[2]);
  doc->DeleteNode(comment);
  doc->Print();
  cout << "\n\n";

  element->FirstChildElement()->SetAttribute("attrib", true);
  element->LastChildElement()->DeleteAttribute("attrib");
  doc->Print();
  cout << "\n\n";

  XMLTest("Programmatic DOM", true, doc->FirstChildElement()->FirstChildElement()->BoolAttribute("attrib"));
  const int defaultIntValue = 10;
  const int replacementIntValue = 20;
  int value1 = defaultIntValue;
  int value2 = doc->FirstChildElement()->LastChildElement()->IntAttribute("attrib", replacementIntValue);
  XMLError result = doc->FirstChildElement()->LastChildElement()->QueryIntAttribute("attrib", &value1);
  XMLTest("Programmatic DOM", XML_NO_ATTRIBUTE, result);
  XMLTest("Programmatic DOM", defaultIntValue, value1);
  XMLTest("Programmatic DOM", replacementIntValue, value2);

  doc->Print();
  cout << "\n\n";

  // {
  //   XMLPrinter streamer;
  //   doc->Print(&streamer);
  //   printf("%s", streamer.CStr());
  // }
  // {
  //   XMLPrinter streamer(0, true);
  //   doc->Print(&streamer);
  //   XMLTest("Compact mode", "<element><sub attrib=\"true\"/><sub/></element>", streamer.CStr(), false);
  // }
  doc->SaveFile("./data/pretty.xml");
  XMLTest("Save pretty.xml", false, doc->Error());
  doc->SaveFile("./data/compact.xml", true);
  XMLTest("Save compact.xml", false, doc->Error());
  delete doc;
}

void test9()
{
  // Test: Dream
  // XML1 : 1,187,569 bytes	in 31,209 allocations
  // XML2 :   469,073	bytes	in    323 allocations
  // int newStart = gNew;
  XMLDocument doc;
  doc.LoadFile("./data/dream.xml");
  XMLTest("Load dream.xml", false, doc.Error());

  doc.SaveFile("./out/dreamout.xml");
  XMLTest("Save dreamout.xml", false, doc.Error());
  doc.PrintError();

  XMLTest("Dream", "xml version=\"1.0\"",
          doc.FirstChild()->ToDeclaration()->Value());
  XMLTest("Dream", true, doc.FirstChild()->NextSibling()->ToUnknown() != 0);
  XMLTest("Dream", "DOCTYPE PLAY SYSTEM \"play.dtd\"",
          doc.FirstChild()->NextSibling()->ToUnknown()->Value());
  XMLTest("Dream", "And Robin shall restore amends.",
          doc.LastChild()->LastChild()->LastChild()->LastChild()->LastChildElement()->GetText());
  XMLTest("Dream", "And Robin shall restore amends.",
          doc.LastChild()->LastChild()->LastChild()->LastChild()->LastChildElement()->GetText());

  XMLDocument doc2;
  doc2.LoadFile("./out/dreamout.xml");
  XMLTest("Load dreamout.xml", false, doc2.Error());
  XMLTest("Dream-out", "xml version=\"1.0\"",
          doc2.FirstChild()->ToDeclaration()->Value());
  XMLTest("Dream-out", true, doc2.FirstChild()->NextSibling()->ToUnknown() != 0);
  XMLTest("Dream-out", "DOCTYPE PLAY SYSTEM \"play.dtd\"",
          doc2.FirstChild()->NextSibling()->ToUnknown()->Value());
  XMLTest("Dream-out", "And Robin shall restore amends.",
          doc2.LastChild()->LastChild()->LastChild()->LastChild()->LastChildElement()->GetText());

  // gNewTotal = gNew - newStart;
}

void test10()
{
  const char *error = "<?xml version=\"1.0\" standalone=\"no\" ?>\n"
                      "<passages count=\"006\" formatversion=\"20020620\">\n"
                      "    <wrong error>\n"
                      "</passages>";

  XMLDocument doc;
  doc.Parse(error);
  XMLTest("Bad XML", XML_ERROR_PARSING_ATTRIBUTE, doc.ErrorID());
  const char *errorStr = doc.ErrorStr();
  XMLTest("Formatted error string",
          "Error=XML_ERROR_PARSING_ATTRIBUTE ErrorID=7 (0x7) Line number=3: XMLElement name=wrong",
          errorStr);
}

/// 解析数字，并进行类型转换
void test11()
{
  const char *str = "<doc attr0='1' attr1='2.0' attr2='foo' />";

  XMLDocument doc;
  doc.Parse(str);
  XMLTest("Top level attributes", false, doc.Error());

  XMLElement *ele = doc.FirstChildElement();

  int iVal;
  XMLError result;
  double dVal;

  result = ele->QueryDoubleAttribute("attr0", &dVal);
  XMLTest("Query attribute: int as double", XML_SUCCESS, result);
  XMLTest("Query attribute: int as double", 1, (int)dVal);
  XMLTest("Query attribute: int as double", 1, (int)ele->DoubleAttribute("attr0"));

  result = ele->QueryDoubleAttribute("attr1", &dVal);
  XMLTest("Query attribute: double as double", XML_SUCCESS, result);
  XMLTest("Query attribute: double as double", 2.0, dVal);
  XMLTest("Query attribute: double as double", 2.0, ele->DoubleAttribute("attr1"));

  result = ele->QueryIntAttribute("attr1", &iVal);
  XMLTest("Query attribute: double as int", XML_SUCCESS, result);
  XMLTest("Query attribute: double as int", 2, iVal);

  result = ele->QueryIntAttribute("attr2", &iVal);
  XMLTest("Query attribute: not a number", XML_WRONG_ATTRIBUTE_TYPE, result);
  XMLTest("Query attribute: not a number", 4.0, ele->DoubleAttribute("attr2", 4.0));

  result = ele->QueryIntAttribute("bar", &iVal);
  XMLTest("Query attribute: does not exist", XML_NO_ATTRIBUTE, result);
  XMLTest("Query attribute: does not exist", true, ele->BoolAttribute("bar", true));
}

/// 解析int double char类型
void test12()
{
  const char *str = "<doc/>";

  XMLDocument doc;
  doc.Parse(str);
  XMLTest("Empty top element", false, doc.Error());

  XMLElement *ele = doc.FirstChildElement();

  int iVal, iVal2;
  double dVal, dVal2;

  ele->SetAttribute("str", "strValue");
  ele->SetAttribute("int", 1);
  ele->SetAttribute("double", -1.0);

  // 解析字符串
  const char *answer = 0;
  ele->QueryAttribute("str", &answer);
  XMLTest("Query char attribute", "strValue", answer);

  const char *cStr = ele->Attribute("str");
  {
    XMLError queryResult = ele->QueryIntAttribute("int", &iVal); // iVal = 1
    XMLTest("Query int attribute", XML_SUCCESS, queryResult);
  }
  {
    XMLError queryResult = ele->QueryDoubleAttribute("double", &dVal); // dVal = -1.0
    XMLTest("Query double attribute", XML_SUCCESS, queryResult);
  }

  {
    XMLError queryResult = ele->QueryAttribute("int", &iVal2);
    XMLTest("Query int attribute generic", (int)XML_SUCCESS, queryResult);
  }
  {
    XMLError queryResult = ele->QueryAttribute("double", &dVal2);
    XMLTest("Query double attribute generic", (int)XML_SUCCESS, queryResult);
  }

  XMLTest("Attribute match test", "strValue", ele->Attribute("str", "strValue"));
  XMLTest("Attribute round trip. c-string.", "strValue", cStr);
  XMLTest("Attribute round trip. int.", 1, iVal);
  XMLTest("Attribute round trip. double.", -1, (int)dVal);
  XMLTest("Alternate query", true, iVal == iVal2);
  XMLTest("Alternate query", true, dVal == dVal2);
  XMLTest("Alternate query", true, iVal == ele->IntAttribute("int"));
  XMLTest("Alternate query", true, dVal == ele->DoubleAttribute("double"));
}

/// 读取xml文件的utf-8形式内容并保存
void test13()
{
  XMLDocument doc;
  doc.LoadFile("./data/utf8test.xml");
  XMLTest("Load utf8test.xml", false, doc.Error());

  // Get the attribute "value" from the "Russian" element and check it.
  XMLElement *element = doc.FirstChildElement("document")->FirstChildElement("Russian");
  const unsigned char correctValue[] = {0xd1U, 0x86U, 0xd0U, 0xb5U, 0xd0U, 0xbdU, 0xd0U, 0xbdU,
                                        0xd0U, 0xbeU, 0xd1U, 0x81U, 0xd1U, 0x82U, 0xd1U, 0x8cU, 0};

  XMLTest("UTF-8: Russian value.", (const char *)correctValue, element->Attribute("value"));

  // Russian的utf-8表示
  const unsigned char russianElementName[] = {0xd0U, 0xa0U, 0xd1U, 0x83U,
                                              0xd1U, 0x81U, 0xd1U, 0x81U,
                                              0xd0U, 0xbaU, 0xd0U, 0xb8U,
                                              0xd0U, 0xb9U, 0};
  const char russianText[] = "<\xD0\xB8\xD0\xBC\xD0\xB5\xD0\xB5\xD1\x82>"; // имеет的utf-8表示

  XMLText *text = doc.FirstChildElement("document")->FirstChildElement((const char *)russianElementName)->FirstChild()->ToText();
  XMLTest("UTF-8: Browsing russian element name.",
          russianText,
          text->Value());

  // Now try for a round trip.
  doc.SaveFile("./out/utf8testout.xml");
  XMLTest("UTF-8: Save testout.xml", false, doc.Error());

  // Check the round trip.
  bool roundTripOkay = false;

  FILE *saved = fopen("./out/utf8testout.xml", "r");
  XMLTest("UTF-8: Open utf8testout.xml", true, saved != 0);

  FILE *verify = fopen("./data/utf8testverify.xml", "r");
  XMLTest("UTF-8: Open utf8testverify.xml", true, verify != 0);

  if (saved && verify)
  {
    roundTripOkay = true;
    char verifyBuf[256];
    while (fgets(verifyBuf, 256, verify))
    {
      char savedBuf[256];
      fgets(savedBuf, 256, saved);
      NullLineEndings(verifyBuf);
      NullLineEndings(savedBuf);

      if (strcmp(verifyBuf, savedBuf))
      {
        printf("verify:%s<\n", verifyBuf);
        printf("saved :%s<\n", savedBuf);
        roundTripOkay = false;
        break;
      }
    }
  }
  if (saved)
    fclose(saved);
  if (verify)
    fclose(verify);
  XMLTest("UTF-8: Verified multi-language round trip.", true, roundTripOkay);
}

/// 取得元素中间的文字
void test14()
{
  const char *str = "<foo>This is  text</foo>";
  XMLDocument doc;
  doc.Parse(str);
  XMLTest("Double whitespace", false, doc.Error());
  const XMLElement *element = doc.RootElement();

  XMLTest("GetText() normal use.", "This is  text", element->GetText());

  str = "<foo><b>This is text</b></foo>"; // 粗体显示字符
  doc.Parse(str);
  XMLTest("Bold text simulation", false, doc.Error());
  element = doc.RootElement();

  XMLTest("GetText() contained element.", element->GetText() == 0, true);
}

/// 设置元素之间的文本
void test15()
{
  const char *str = "<foo></foo>";
  XMLDocument doc;
  doc.Parse(str);
  XMLTest("Empty closed element", false, doc.Error());
  XMLElement *element = doc.RootElement();

  element->SetText("darkness.");
  XMLTest("SetText() normal use (open/close).", "darkness.", element->GetText());

  element->SetText("blue flame.");
  XMLTest("SetText() replace.", "blue flame.", element->GetText());

  str = "<foo/>";
  doc.Parse(str);
  XMLTest("Empty self-closed element", false, doc.Error());
  element = doc.RootElement();

  element->SetText("The driver");
  XMLTest("SetText() normal use. (self-closing)", "The driver", element->GetText());

  element->SetText("<b>horses</b>");
  XMLTest("SetText() replace with tag-like text.", "<b>horses</b>", element->GetText());
  // doc.Print();

  str = "<foo><bar>Text in nested element</bar></foo>";
  doc.Parse(str);
  XMLTest("Text in nested element", false, doc.Error());
  element = doc.RootElement();

  element->SetText("wolves");
  XMLTest("SetText() prefix to nested non-text children.", "wolves", element->GetText());

  str = "<foo/>";
  doc.Parse(str);
  XMLTest("Empty self-closed element round 2", false, doc.Error());
  element = doc.RootElement();

  element->SetText("str");
  XMLTest("SetText types", "str", element->GetText());

  element->SetText(1);
  XMLTest("SetText types", "1", element->GetText());

  element->SetText(1U);
  XMLTest("SetText types", "1", element->GetText());

  element->SetText(true);
  XMLTest("SetText types", "true", element->GetText());

  element->SetText(1.5f);
  XMLTest("SetText types", "1.5", element->GetText());

  element->SetText(1.5);
  XMLTest("SetText types", "1.5", element->GetText());
}

/// 解析各种类型的Attribute
void test16()
{
  static const int64_t BIG = -123456789012345678;
  static const uint64_t BIG_POS = 123456789012345678;
  XMLDocument doc;
  XMLElement *element = doc.NewElement("element");
  doc.InsertFirstChild(element);

  {
    element->SetAttribute("attrib", int(-100));
    {
      int v = 0;
      XMLError queryResult = element->QueryIntAttribute("attrib", &v);
      XMLTest("Attribute: int", XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: int", -100, v, true);
    }
    {
      int v = 0;
      XMLError queryResult = element->QueryAttribute("attrib", &v);
      XMLTest("Attribute: int", (int)XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: int", -100, v, true);
    }
    XMLTest("Attribute: int", -100, element->IntAttribute("attrib"), true);
  }
  {
    element->SetAttribute("attrib", unsigned(100));
    {
      unsigned v = 0;
      XMLError queryResult = element->QueryUnsignedAttribute("attrib", &v);
      XMLTest("Attribute: unsigned", XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: unsigned", unsigned(100), v, true);
    }
    {
      unsigned v = 0;
      XMLError queryResult = element->QueryAttribute("attrib", &v);
      XMLTest("Attribute: unsigned", (int)XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: unsigned", unsigned(100), v, true);
    }
    {
      const char *v = "failed";
      XMLError queryResult = element->QueryStringAttribute("not-attrib", &v);
      XMLTest("Attribute: string default", false, queryResult == XML_SUCCESS);
      queryResult = element->QueryStringAttribute("attrib", &v);
      XMLTest("Attribute: string", XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: string", "100", v);
    }
    XMLTest("Attribute: unsigned", unsigned(100), element->UnsignedAttribute("attrib"), true);
  }
  {
    element->SetAttribute("attrib", BIG);
    {
      int64_t v = 0;
      XMLError queryResult = element->QueryInt64Attribute("attrib", &v);
      XMLTest("Attribute: int64_t", XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: int64_t", BIG, v, true);
    }
    {
      int64_t v = 0;
      XMLError queryResult = element->QueryAttribute("attrib", &v);
      XMLTest("Attribute: int64_t", (int)XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: int64_t", BIG, v, true);
    }
    XMLTest("Attribute: int64_t", BIG, element->Int64Attribute("attrib"), true);
  }
  {
    element->SetAttribute("attrib", BIG_POS);
    {
      uint64_t v = 0;
      XMLError queryResult = element->QueryUnsigned64Attribute("attrib", &v);
      XMLTest("Attribute: uint64_t", XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: uint64_t", BIG_POS, v, true);
    }
    {
      uint64_t v = 0;
      XMLError queryResult = element->QueryAttribute("attrib", &v);
      XMLTest("Attribute: uint64_t", (int)XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: uint64_t", BIG_POS, v, true);
    }
    XMLTest("Attribute: uint64_t", BIG_POS, element->Unsigned64Attribute("attrib"), true);
  }
  {
    element->SetAttribute("attrib", true);
    {
      bool v = false;
      XMLError queryResult = element->QueryBoolAttribute("attrib", &v);
      XMLTest("Attribute: bool", XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: bool", true, v, true);
    }
    {
      bool v = false;
      XMLError queryResult = element->QueryAttribute("attrib", &v);
      XMLTest("Attribute: bool", (int)XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: bool", true, v, true);
    }
    XMLTest("Attribute: bool", true, element->BoolAttribute("attrib"), true);
  }
  {
    element->SetAttribute("attrib", true);
    const char *result = element->Attribute("attrib");
    XMLTest("Bool true is 'true'", "true", result);

    XMLUtil::SetBoolSerialization("1", "0");
    element->SetAttribute("attrib", true);
    result = element->Attribute("attrib");
    XMLTest("Bool true is '1'", "1", result);

    XMLUtil::SetBoolSerialization(0, 0);
  }
  {
    element->SetAttribute("attrib", 100.0);
    {
      double v = 0;
      XMLError queryResult = element->QueryDoubleAttribute("attrib", &v);
      XMLTest("Attribute: double", XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: double", 100.0, v, true);
    }
    {
      double v = 0;
      XMLError queryResult = element->QueryAttribute("attrib", &v);
      XMLTest("Attribute: bool", (int)XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: double", 100.0, v, true);
    }
    XMLTest("Attribute: double", 100.0, element->DoubleAttribute("attrib"), true);
  }
  {
    element->SetAttribute("attrib", 100.0f);
    {
      float v = 0;
      XMLError queryResult = element->QueryFloatAttribute("attrib", &v);
      XMLTest("Attribute: float", XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: float", 100.0f, v, true);
    }
    {
      float v = 0;
      XMLError queryResult = element->QueryAttribute("attrib", &v);
      XMLTest("Attribute: float", (int)XML_SUCCESS, queryResult, true);
      XMLTest("Attribute: float", 100.0f, v, true);
    }
    XMLTest("Attribute: float", 100.0f, element->FloatAttribute("attrib"), true);
  }
  {
    element->SetText(BIG);
    int64_t v = 0;
    XMLError queryResult = element->QueryInt64Text(&v);
    XMLTest("Element: int64_t", XML_SUCCESS, queryResult, true);
    XMLTest("Element: int64_t", BIG, v, true);
  }
  {
    element->SetText(BIG_POS);
    uint64_t v = 0;
    XMLError queryResult = element->QueryUnsigned64Text(&v);
    XMLTest("Element: uint64_t", XML_SUCCESS, queryResult, true);
    XMLTest("Element: uint64_t", BIG_POS, v, true);
  }
}

/// ---------- XMLPrinter stream mode ------
void test17()
{
  {
    FILE *printerfp = fopen("./out/printer.xml", "w");
    XMLTest("Open printer.xml", true, printerfp != 0);
    XMLPrinter printer(printerfp);
    printer.OpenElement("foo");
    printer.PushAttribute("attrib-text", "text");
    printer.PushAttribute("attrib-int", int(1));
    printer.PushAttribute("attrib-unsigned", unsigned(2));
    printer.PushAttribute("attrib-int64", int64_t(3));
    printer.PushAttribute("attrib-uint64", uint64_t(37));
    printer.PushAttribute("attrib-bool", true);
    printer.PushAttribute("attrib-double", 4.0);
    printer.CloseElement();
    fclose(printerfp);
  }
  {
    XMLDocument doc;
    doc.LoadFile("./out/printer.xml");
    XMLTest("XMLPrinter Stream mode: load", XML_SUCCESS, doc.ErrorID(), true);

    const XMLDocument &cdoc = doc;

    const XMLAttribute *attrib = cdoc.FirstChildElement("foo")->FindAttribute("attrib-text");
    XMLTest("attrib-text", "text", attrib->Value(), true);
    attrib = cdoc.FirstChildElement("foo")->FindAttribute("attrib-int");
    XMLTest("attrib-int", int(1), attrib->IntValue(), true);
    attrib = cdoc.FirstChildElement("foo")->FindAttribute("attrib-unsigned");
    XMLTest("attrib-unsigned", unsigned(2), attrib->UnsignedValue(), true);
    attrib = cdoc.FirstChildElement("foo")->FindAttribute("attrib-int64");
    XMLTest("attrib-int64", int64_t(3), attrib->Int64Value(), true);
    attrib = cdoc.FirstChildElement("foo")->FindAttribute("attrib-uint64");
    XMLTest("attrib-uint64", uint64_t(37), attrib->Unsigned64Value(), true);
    attrib = cdoc.FirstChildElement("foo")->FindAttribute("attrib-bool");
    XMLTest("attrib-bool", true, attrib->BoolValue(), true);
    attrib = cdoc.FirstChildElement("foo")->FindAttribute("attrib-double");
    XMLTest("attrib-double", 4.0, attrib->DoubleValue(), true);
  }
  // Add API_testcatse :PushDeclaration();PushText();PushComment()
  {
    FILE *fp1 = fopen("./out/printer_1.xml", "w");
    XMLPrinter printer(fp1);

    printer.PushDeclaration("version = '1.0' enconding = 'utf-8'");

    printer.OpenElement("foo");
    printer.PushAttribute("attrib-text", "text");

    printer.OpenElement("text");
    printer.PushText("Tinyxml2");
    printer.CloseElement();

    printer.OpenElement("int");
    printer.PushText(int(11));
    printer.CloseElement();

    printer.OpenElement("unsigned");
    printer.PushText(unsigned(12));
    printer.CloseElement();

    printer.OpenElement("int64_t");
    printer.PushText(int64_t(13));
    printer.CloseElement();

    printer.OpenElement("uint64_t");
    printer.PushText(uint64_t(14));
    printer.CloseElement();

    printer.OpenElement("bool");
    printer.PushText(true);
    printer.CloseElement();

    printer.OpenElement("float");
    printer.PushText("1.56");
    printer.CloseElement();

    printer.OpenElement("double");
    printer.PushText("12.12");
    printer.CloseElement();

    printer.OpenElement("comment");
    printer.PushComment("this is Tinyxml2");
    printer.CloseElement();

    printer.CloseElement();
    fclose(fp1);
  }
  {
    XMLDocument doc;
    doc.LoadFile("./out/printer_1.xml");
    XMLTest("XMLPrinter Stream mode: load", XML_SUCCESS, doc.ErrorID(), true);

    const XMLDocument &cdoc = doc;

    const XMLElement *root = cdoc.FirstChildElement("foo");

    const char *text_value;
    text_value = root->FirstChildElement("text")->GetText();
    XMLTest("PushText( const char* text, bool cdata=false ) test", "Tinyxml2", text_value);

    int int_value;
    int_value = root->FirstChildElement("int")->IntText();
    XMLTest("PushText( int value ) test", 11, int_value);

    unsigned unsigned_value;
    unsigned_value = root->FirstChildElement("unsigned")->UnsignedText();
    XMLTest("PushText( unsigned value ) test", (unsigned)12, unsigned_value);

    int64_t int64_t_value;
    int64_t_value = root->FirstChildElement("int64_t")->Int64Text();
    XMLTest("PushText( int64_t value ) test", (int64_t)13, int64_t_value);

    uint64_t uint64_t_value;
    uint64_t_value = root->FirstChildElement("uint64_t")->Unsigned64Text();
    XMLTest("PushText( uint64_t value ) test", (uint64_t)14, uint64_t_value);

    float float_value;
    float_value = root->FirstChildElement("float")->FloatText();
    XMLTest("PushText( float value ) test", 1.56f, float_value);

    double double_value;
    double_value = root->FirstChildElement("double")->DoubleText();
    XMLTest("PushText( double value ) test", 12.12, double_value);

    bool bool_value;
    bool_value = root->FirstChildElement("bool")->BoolText();
    XMLTest("PushText( bool value ) test", true, bool_value);

    const XMLComment *comment = root->FirstChildElement("comment")->FirstChild()->ToComment();
    const char *comment_value = comment->Value();
    XMLTest("PushComment() test", "this is Tinyxml2", comment_value);

    const XMLDeclaration *declaration = cdoc.FirstChild()->ToDeclaration();
    const char *declaration_value = declaration->Value();
    XMLTest("PushDeclaration() test", "version = '1.0' enconding = 'utf-8'", declaration_value);
  }
}

void test18()
{
  const char *str = "<xmlElement>"
                    "<![CDATA["
                    "I am > the rules!\n"
                    "...since I make symbolic puns"
                    "]]>"
                    "</xmlElement>";
  XMLDocument doc;
  doc.Parse(str);
  XMLTest("CDATA symbolic puns round 1", false, doc.Error());
  doc.Print();

  XMLTest("CDATA parse.", "I am > the rules!\n...since I make symbolic puns",
          doc.FirstChildElement()->FirstChild()->Value(),
          false);
}

void test19()
{
  const char *str = "<xmlElement>"
                    "<![CDATA["
                    "<b>I am > the rules!</b>\n"
                    "...since I make symbolic puns"
                    "]]>"
                    "</xmlElement>";
  XMLDocument doc;
  doc.Parse(str);
  XMLTest("CDATA symbolic puns round 2", false, doc.Error());
  doc.Print();

  XMLTest("CDATA parse. [ tixml1:1480107 ]",
          "<b>I am > the rules!</b>\n...since I make symbolic puns",
          doc.FirstChildElement()->FirstChild()->Value(),
          false);
}

void test20()
{
  // InsertBeforeChild and InsertAfterChild causes crash.
  XMLDocument doc;
  XMLElement *parent = doc.NewElement("Parent");
  doc.InsertFirstChild(parent);

  XMLElement *childText0 = doc.NewElement("childText0");
  XMLElement *childText1 = doc.NewElement("childText1");

  XMLNode *childNode0 = parent->InsertEndChild(childText0);
  XMLTest("InsertEndChild() return", true, childNode0 == childText0);
  XMLNode *childNode1 = parent->InsertAfterChild(childNode0, childText1);
  XMLTest("InsertAfterChild() return", true, childNode1 == childText1);

  XMLTest("Test InsertAfterChild on empty node. ", true, (childNode1 == parent->LastChild()));
}

void test21()
{
  // Entities not being written correctly.
  // From Lynn Allen

  const char *passages =
      "<?xml version=\"1.0\" standalone=\"no\" ?>"
      "<passages count=\"006\" formatversion=\"20020620\">"
      "<psg context=\"Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;."
      " It also has &lt;, &gt;, and &amp;, as well as a fake copyright &#xA9;.\"> </psg>"
      "</passages>";

  XMLDocument doc;
  doc.Parse(passages);
  XMLTest("Entity transformation parse round 1", false, doc.Error());
  XMLElement *psg = doc.RootElement()->FirstChildElement();
  const char *context = psg->Attribute("context");
  const char *expected = "Line 5 has \"quotation marks\" and 'apostrophe marks'. It also has <, >, and &, as well as a fake copyright \xC2\xA9.";

  XMLTest("Entity transformation: read. ", expected, context, true);

  const char *textFilePath = "./out/textfile.txt";
  FILE *textfile = fopen(textFilePath, "w");
  XMLTest("Entity transformation: open text file for writing", true, textfile != 0, true);
  if (textfile)
  {
    XMLPrinter streamer(textfile);
    bool acceptResult = psg->Accept(&streamer); // 写到文件中
    fclose(textfile);
    XMLTest("Entity transformation: Accept", true, acceptResult);
  }

  textfile = fopen(textFilePath, "r");
  XMLTest("Entity transformation: open text file for reading", true, textfile != 0, true);
  if (textfile)
  {
    char buf[1024];
    fgets(buf, 1024, textfile);
    XMLTest("Entity transformation: write. ",
            "<psg context=\"Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;."
            " It also has &lt;, &gt;, and &amp;, as well as a fake copyright \xC2\xA9.\"/>\n",
            buf, false);
    fclose(textfile);
  }
}

void test22()
{
  // Suppress entities.
  const char *passages =
      "<?xml version=\"1.0\" standalone=\"no\" ?>"
      "<passages count=\"006\" formatversion=\"20020620\">"
      "<psg context=\"Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;.\">Crazy &ttk;</psg>"
      "</passages>";

  XMLDocument doc(false);
  doc.Parse(passages);
  XMLTest("Entity transformation parse round 2", false, doc.Error());

  XMLTest("No entity parsing.",
          "Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;.",
          doc.FirstChildElement()->FirstChildElement()->Attribute("context"));
  XMLTest("No entity parsing.", "Crazy &ttk;",
          doc.FirstChildElement()->FirstChildElement()->FirstChild()->Value());
  doc.Print();
}

void test23()
{
  // [ 1663758 ] Failure to report error on bad XML
  XMLDocument xml;
  xml.Parse("<x>");
  XMLTest("Missing end tag at end of input", true, xml.Error());
  xml.Parse("<x> ");
  XMLTest("Missing end tag with trailing whitespace", true, xml.Error());
  xml.Parse("<x></y>");
  XMLTest("Mismatched tags", XML_ERROR_MISMATCHED_ELEMENT, xml.ErrorID());
}

/// 解析comment
void test24()
{
  // [ 1475201 ] TinyXML parses entities in comments
  XMLDocument xml;
  xml.Parse("<!-- declarations for <head> & <body> -->"
            "<!-- far &amp; away -->");
  XMLTest("Declarations for head and body", false, xml.Error());

  XMLNode *e0 = xml.FirstChild();
  XMLNode *e1 = e0->NextSibling();
  XMLComment *c0 = e0->ToComment();
  XMLComment *c1 = e1->ToComment();

  XMLTest("Comments ignore entities.", " declarations for <head> & <body> ", c0->Value(), true);
  XMLTest("Comments ignore entities.", " far &amp; away ", c1->Value(), true);
}

/// 循环处理一个结点
void test25()
{
  XMLDocument xml;
  xml.Parse("<Parent>"
            "<child1 att=''/>"
            "<!-- With this comment, child2 will not be parsed! -->"
            "<child2 att=''/>"
            "</Parent>");
  XMLTest("Comments iteration", false, xml.Error());
  xml.Print();

  int count = 0;

  for (XMLNode *ele = xml.FirstChildElement("Parent")->FirstChild();
       ele;
       ele = ele->NextSibling())
  {
    ++count;
  }

  XMLTest("Comments iterate correctly.", 3, count);
}

/// 克隆一份结点
void test26()
{
  const char *pub = "<?xml version='1.0'?> <element><sub/></element> <!--comment--> <!DOCTYPE>";
  XMLDocument doc;
  doc.Parse(pub);
  XMLTest("Trailing DOCTYPE", false, doc.Error());

  XMLDocument clone;
  for (const XMLNode *node = doc.FirstChild(); node; node = node->NextSibling())
  {
    XMLNode *copy = node->ShallowClone(&clone);
    clone.InsertEndChild(copy);
  }

  clone.Print();

  int count = 0;
  const XMLNode *a = clone.FirstChild();
  const XMLNode *b = doc.FirstChild();
  for (; a && b; a = a->NextSibling(), b = b->NextSibling())
  {
    ++count;
    XMLTest("Clone and Equal", true, a->ShallowEqual(b));
  }
  XMLTest("Clone and Equal", 4, count);
}

void test27()
{
  // Deep Cloning of root element.
  XMLDocument doc2;
  XMLPrinter printer1;
  {
    // Make sure doc1 is deleted before we test doc2
    const char *xml =
        "<root>"
        "    <child1 foo='bar'/>"
        "    <!-- comment thing -->"
        "    <child2 val='1'>Text</child2>"
        "</root>";
    XMLDocument doc;
    doc.Parse(xml);
    XMLTest("Parse before deep cloning root element", false, doc.Error());

    doc.Print(&printer1);
    XMLNode *root = doc.RootElement()->DeepClone(&doc2);
    doc2.InsertFirstChild(root);
  }
  XMLPrinter printer2;
  doc2.Print(&printer2);

  XMLTest("Deep clone of element.", printer1.CStr(), printer2.CStr(), true);
}

void test28()
{
  // Deep Cloning of sub element.
  XMLDocument doc2;
  XMLPrinter printer1;
  {
    // Make sure doc1 is deleted before we test doc2
    const char *xml =
        "<?xml version ='1.0'?>"
        "<root>"
        "    <child1 foo='bar'/>"
        "    <!-- comment thing -->"
        "    <child2 val='1'>Text</child2>"
        "</root>";
    XMLDocument doc;
    doc.Parse(xml);
    XMLTest("Parse before deep cloning sub element", false, doc.Error());

    const XMLElement *subElement = doc.FirstChildElement("root")->FirstChildElement("child2");
    bool acceptResult = subElement->Accept(&printer1);
    XMLTest("Accept before deep cloning", true, acceptResult);

    XMLNode *clonedSubElement = subElement->DeepClone(&doc2);
    doc2.InsertFirstChild(clonedSubElement);
  }
  XMLPrinter printer2;
  doc2.Print(&printer2);

  XMLTest("Deep clone of sub-element.", printer1.CStr(), printer2.CStr(), true);
}

void test29()
{
  // Deep cloning of document.
  XMLDocument doc2;
  XMLPrinter printer1;
  {
    // Make sure doc1 is deleted before we test doc2
    const char *xml =
        "<?xml version ='1.0'?>"
        "<!-- Top level comment. -->"
        "<root>"
        "    <child1 foo='bar'/>"
        "    <!-- comment thing -->"
        "    <child2 val='1'>Text</child2>"
        "</root>";
    XMLDocument doc;
    doc.Parse(xml);
    XMLTest("Parse before deep cloning document", false, doc.Error());
    doc.Print(&printer1);

    doc.DeepCopy(&doc2);
  }
  XMLPrinter printer2;
  doc2.Print(&printer2);

  XMLTest("DeepCopy of document.", printer1.CStr(), printer2.CStr(), true);
}

void test30()
{
  // Attribute ordering.
  static const char *xml = "<element attrib1=\"1\" attrib2=\"2\" attrib3=\"3\" />";
  XMLDocument doc;
  doc.Parse(xml);
  XMLTest("Parse for attribute ordering", false, doc.Error());
  XMLElement *ele = doc.FirstChildElement();

  const XMLAttribute *a = ele->FirstAttribute();
  XMLTest("Attribute order", "1", a->Value());
  a = a->Next();
  XMLTest("Attribute order", "2", a->Value());
  a = a->Next();
  XMLTest("Attribute order", "3", a->Value());
  XMLTest("Attribute order", "attrib3", a->Name());

  ele->DeleteAttribute("attrib2");
  a = ele->FirstAttribute();
  XMLTest("Attribute order", "1", a->Value());
  a = a->Next();
  XMLTest("Attribute order", "3", a->Value());

  ele->DeleteAttribute("attrib1");
  ele->DeleteAttribute("attrib3");
  XMLTest("Attribute order (empty)", true, ele->FirstAttribute() == 0);
}

void test31()
{
  // Make sure an attribute with a space in it succeeds.
  static const char *xml0 = "<element attribute1= \"Test Attribute\"/>";
  static const char *xml1 = "<element attribute1 =\"Test Attribute\"/>";
  static const char *xml2 = "<element attribute1 = \"Test Attribute\"/>";
  XMLDocument doc0;
  doc0.Parse(xml0);
  XMLTest("Parse attribute with space 1", false, doc0.Error());
  XMLDocument doc1;
  doc1.Parse(xml1);
  XMLTest("Parse attribute with space 2", false, doc1.Error());
  XMLDocument doc2;
  doc2.Parse(xml2);
  XMLTest("Parse attribute with space 3", false, doc2.Error());

  XMLElement *ele = 0;
  ele = doc0.FirstChildElement();
  XMLTest("Attribute with space #1", "Test Attribute", ele->Attribute("attribute1"));
  ele = doc1.FirstChildElement();
  XMLTest("Attribute with space #2", "Test Attribute", ele->Attribute("attribute1"));
  ele = doc2.FirstChildElement();
  XMLTest("Attribute with space #3", "Test Attribute", ele->Attribute("attribute1"));
}

/// 判断当前element和后一个element是否完全相同
void test32()
{
  // Make sure we don't go into an infinite loop.
  static const char *xml = "<doc><element attribute='attribute'/><element attribute='attribute'/></doc>";
  XMLDocument doc;
  doc.Parse(xml);
  XMLTest("Parse two elements with attribute", false, doc.Error());
  XMLElement *ele0 = doc.FirstChildElement()->FirstChildElement();
  XMLElement *ele1 = ele0->NextSiblingElement();
  bool equal = ele0->ShallowEqual(ele1);

  XMLTest("Infinite loop in shallow equal.", true, equal);
}

/// 解析标签
void test33()
{
  static const char *xml = "<element attrib='bar'><sub>Text</sub></element>";
  XMLDocument doc;
  doc.Parse(xml);
  XMLTest("Handle, parse element with attribute and nested element", false, doc.Error());

  {
    XMLElement *ele = XMLHandle(doc).FirstChildElement("element").FirstChild().ToElement();
    XMLTest("Handle, non-const, element is found", true, ele != 0);
    XMLTest("Handle, non-const, element name matches", "sub", ele->Value());
  }

  {
    XMLHandle docH(doc);
    XMLElement *ele = docH.FirstChildElement("noSuchElement").FirstChildElement("element").ToElement();
    XMLTest("Handle, non-const, element not found", true, ele == 0);
  }

  {
    const XMLElement *ele = XMLConstHandle(doc).FirstChildElement("element").FirstChild().ToElement();
    XMLTest("Handle, const, element is found", true, ele != 0);
    XMLTest("Handle, const, element name matches", "sub", ele->Value());
  }

  {
    XMLConstHandle docH(doc);
    const XMLElement *ele = docH.FirstChildElement("noSuchElement").FirstChildElement("element").ToElement();
    XMLTest("Handle, const, element not found", true, ele == 0);
  }
}

void test34()
{
  // API:IntText(),UnsignedText(),Int64Text(),DoubleText(),BoolText() and FloatText() test
  const char *xml = "<point> <IntText>-24</IntText> <UnsignedText>42</UnsignedText> \
						   <Int64Text>38</Int64Text> <BoolText>true</BoolText> <DoubleText>2.35</DoubleText> </point>";
  XMLDocument doc;
  doc.Parse(xml);

  const XMLElement *pointElement = doc.RootElement();
  int test1 = pointElement->FirstChildElement("IntText")->IntText();
  XMLTest("IntText() test", -24, test1);

  unsigned test2 = pointElement->FirstChildElement("UnsignedText")->UnsignedText();
  XMLTest("UnsignedText() test", static_cast<unsigned>(42), test2);

  int64_t test3 = pointElement->FirstChildElement("Int64Text")->Int64Text();
  XMLTest("Int64Text() test", static_cast<int64_t>(38), test3);

  double test4 = pointElement->FirstChildElement("DoubleText")->DoubleText();
  XMLTest("DoubleText() test", 2.35, test4);

  float test5 = pointElement->FirstChildElement("DoubleText")->FloatText();
  XMLTest("FloatText()) test", 2.35f, test5);

  bool test6 = pointElement->FirstChildElement("BoolText")->BoolText();
  XMLTest("FloatText()) test", true, test6);
}

void test35()
{
  // hex value test
  const char *xml = "<point> <IntText>  0x2020</IntText> <UnsignedText>0X2020</UnsignedText> \
						   <Int64Text> 0x1234</Int64Text></point>";
  XMLDocument doc;
  doc.Parse(xml);

  const XMLElement *pointElement = doc.RootElement();
  int test1 = pointElement->FirstChildElement("IntText")->IntText();
  XMLTest("IntText() hex value test", 0x2020, test1);

  unsigned test2 = pointElement->FirstChildElement("UnsignedText")->UnsignedText();
  XMLTest("UnsignedText() hex value test", static_cast<unsigned>(0x2020), test2);

  int64_t test3 = pointElement->FirstChildElement("Int64Text")->Int64Text();
  XMLTest("Int64Text() hex value test", static_cast<int64_t>(0x1234), test3);
}

void test36()
{
  // API:ShallowEqual() test
  const char *xml = "<playlist id = 'playlist'>"
                    "<property name = 'track_name'>voice</property>"
                    "</playlist>";
  XMLDocument doc;
  doc.Parse(xml);
  const XMLNode *PlaylistNode = doc.RootElement();
  const XMLNode *PropertyNode = PlaylistNode->FirstChildElement();
  bool result;
  result = PlaylistNode->ShallowEqual(PropertyNode);
  XMLTest("ShallowEqual() test", false, result);
  result = PlaylistNode->ShallowEqual(PlaylistNode);
  XMLTest("ShallowEqual() test", true, result);
}

void test37()
{
  // API: previousSiblingElement() and NextSiblingElement() test
  const char *xml = "<playlist id = 'playlist'>"
                    "<property name = 'track_name'>voice</property>"
                    "<entry out = '946' producer = '2_playlist1' in = '0'/>"
                    "<blank length = '1'/>"
                    "</playlist>";
  XMLDocument doc;
  doc.Parse(xml);
  XMLElement *ElementPlaylist = doc.FirstChildElement("playlist");
  XMLTest("previousSiblingElement() test", true, ElementPlaylist != 0);
  const XMLElement *pre = ElementPlaylist->PreviousSiblingElement();
  XMLTest("previousSiblingElement() test", true, pre == 0);
  const XMLElement *ElementBlank = ElementPlaylist->FirstChildElement("entry")->NextSiblingElement("blank");
  XMLTest("NextSiblingElement() test", true, ElementBlank != 0);
  const XMLElement *next = ElementBlank->NextSiblingElement();
  XMLTest("NextSiblingElement() test", true, next == 0);
  const XMLElement *ElementEntry = ElementBlank->PreviousSiblingElement("entry");
  XMLTest("PreviousSiblingElement test", true, ElementEntry != 0);
}

void test38()
{
  const char *xml = "<point> <x>1.2</x> <y>1</y> <z>38</z> <valid>true</valid> </point>";
  XMLDocument doc;
  doc.Parse(xml);
  XMLTest("Parse points", false, doc.Error());

  const XMLElement *pointElement = doc.RootElement();

  {
    int intValue = 0;
    XMLError queryResult = pointElement->FirstChildElement("y")->QueryIntText(&intValue);
    XMLTest("QueryIntText result", XML_SUCCESS, queryResult, false);
    XMLTest("QueryIntText", 1, intValue, false);
  }

  {
    unsigned unsignedValue = 0;
    XMLError queryResult = pointElement->FirstChildElement("y")->QueryUnsignedText(&unsignedValue);
    XMLTest("QueryUnsignedText result", XML_SUCCESS, queryResult, false);
    XMLTest("QueryUnsignedText", (unsigned)1, unsignedValue, false);
  }

  {
    float floatValue = 0;
    XMLError queryResult = pointElement->FirstChildElement("x")->QueryFloatText(&floatValue);
    XMLTest("QueryFloatText result", XML_SUCCESS, queryResult, false);
    XMLTest("QueryFloatText", 1.2f, floatValue, false);
  }

  {
    double doubleValue = 0;
    XMLError queryResult = pointElement->FirstChildElement("x")->QueryDoubleText(&doubleValue);
    XMLTest("QueryDoubleText result", XML_SUCCESS, queryResult, false);
    XMLTest("QueryDoubleText", 1.2, doubleValue, false);
  }

  {
    bool boolValue = false;
    XMLError queryResult = pointElement->FirstChildElement("valid")->QueryBoolText(&boolValue);
    XMLTest("QueryBoolText result", XML_SUCCESS, queryResult, false);
    XMLTest("QueryBoolText", true, boolValue, false);
  }
}

void test39()
{
  const char *xml = "<element>"
                    "<a> This \nis &apos;  text  &apos; </a>"
                    "<b>  This is &apos; text &apos;  \n</b>"
                    "<c>This  is  &apos;  \n\n text &apos;</c>"
                    "</element>";
  XMLDocument doc(true, COLLAPSE_WHITESPACE);
  doc.Parse(xml);
  XMLTest("Parse with whitespace collapsing and &apos", false, doc.Error());

  const XMLElement *element = doc.FirstChildElement();
  for (const XMLElement *parent = element->FirstChildElement();
       parent;
       parent = parent->NextSiblingElement())
  {
    XMLTest("Whitespace collapse", "This is ' text '", parent->GetText());
  }
}

void test40()
{
  XMLDocument doc(true, PEDANTIC_WHITESPACE);
  doc.LoadFile("./data/dream.xml");
  XMLTest("Load dream.xml with pedantic whitespace mode", false, doc.Error());

  XMLTest("Dream", "xml version=\"1.0\"",
          doc.FirstChild()->ToDeclaration()->Value());
  XMLTest("Dream", true, doc.FirstChild()->NextSibling()->ToUnknown() != 0);
  XMLTest("Dream", "DOCTYPE PLAY SYSTEM \"play.dtd\"",
          doc.FirstChild()->NextSibling()->ToUnknown()->Value());
  XMLTest("Dream", "And Robin shall restore amends.",
          doc.LastChild()->LastChild()->LastChild()->LastChild()->LastChildElement()->GetText());
}

void test41()
{
  const char *xml = "<parent><child>abc</child></parent>";
  XMLDocument doc;
  doc.Parse(xml);
  XMLTest("Parse for printing of sub-element", false, doc.Error());
  XMLElement *ele = doc.FirstChildElement("parent")->FirstChildElement("child");

  XMLPrinter printer;
  bool acceptResult = ele->Accept(&printer);
  XMLTest("Accept of sub-element", true, acceptResult);
  XMLTest("Printing of sub-element", "<child>abc</child>\n", printer.CStr(), false);
}

void test42()
{
  XMLDocument doc;
  XMLError error = doc.LoadFile("./data/empty.xml");
  XMLTest("Loading an empty file", XML_ERROR_EMPTY_DOCUMENT, error);
  XMLTest("Loading an empty file and ErrorName as string", "XML_ERROR_EMPTY_DOCUMENT", doc.ErrorName());
  doc.PrintError();
}

void test43()
{
  // Insertion with Removal
  const char *xml = "<?xml version=\"1.0\" ?>"
                    "<root>"
                    "<one>"
                    "<subtree>"
                    "<elem>element 1</elem>text<!-- comment -->"
                    "</subtree>"
                    "</one>"
                    "<two/>"
                    "</root>";
  const char *xmlInsideTwo = "<?xml version=\"1.0\" ?>"
                             "<root>"
                             "<one/>"
                             "<two>"
                             "<subtree>"
                             "<elem>element 1</elem>text<!-- comment -->"
                             "</subtree>"
                             "</two>"
                             "</root>";
  const char *xmlAfterOne = "<?xml version=\"1.0\" ?>"
                            "<root>"
                            "<one/>"
                            "<subtree>"
                            "<elem>element 1</elem>text<!-- comment -->"
                            "</subtree>"
                            "<two/>"
                            "</root>";
  const char *xmlAfterTwo = "<?xml version=\"1.0\" ?>"
                            "<root>"
                            "<one/>"
                            "<two/>"
                            "<subtree>"
                            "<elem>element 1</elem>text<!-- comment -->"
                            "</subtree>"
                            "</root>";

  XMLDocument doc;
  doc.Parse(xml);
  XMLTest("Insertion with removal parse round 1", false, doc.Error());
  XMLElement *subtree = doc.RootElement()->FirstChildElement("one")->FirstChildElement("subtree");
  XMLElement *two = doc.RootElement()->FirstChildElement("two");
  two->InsertFirstChild(subtree);
  XMLPrinter printer1(0, true);
  bool acceptResult = doc.Accept(&printer1);
  XMLTest("Move node from within <one> to <two> - Accept()", true, acceptResult);
  XMLTest("Move node from within <one> to <two>", xmlInsideTwo, printer1.CStr());

  doc.Parse(xml);
  XMLTest("Insertion with removal parse round 2", false, doc.Error());
  subtree = doc.RootElement()->FirstChildElement("one")->FirstChildElement("subtree");
  two = doc.RootElement()->FirstChildElement("two");
  doc.RootElement()->InsertAfterChild(two, subtree);
  XMLPrinter printer2(0, true);
  acceptResult = doc.Accept(&printer2);
  XMLTest("Move node from within <one> after <two> - Accept()", true, acceptResult);
  XMLTest("Move node from within <one> after <two>", xmlAfterTwo, printer2.CStr(), false);

  doc.Parse(xml);
  XMLTest("Insertion with removal parse round 3", false, doc.Error());
  XMLNode *one = doc.RootElement()->FirstChildElement("one");
  subtree = one->FirstChildElement("subtree");
  doc.RootElement()->InsertAfterChild(one, subtree);
  XMLPrinter printer3(0, true);
  acceptResult = doc.Accept(&printer3);
  XMLTest("Move node from within <one> after <one> - Accept()", true, acceptResult);
  XMLTest("Move node from within <one> after <one>", xmlAfterOne, printer3.CStr(), false);

  doc.Parse(xml);
  XMLTest("Insertion with removal parse round 4", false, doc.Error());
  subtree = doc.RootElement()->FirstChildElement("one")->FirstChildElement("subtree");
  two = doc.RootElement()->FirstChildElement("two");
  XMLTest("<two> is the last child at root level", true, two == doc.RootElement()->LastChildElement());
  doc.RootElement()->InsertEndChild(subtree);
  XMLPrinter printer4(0, true);
  acceptResult = doc.Accept(&printer4);
  XMLTest("Move node from within <one> after <two> - Accept()", true, acceptResult);
  XMLTest("Move node from within <one> after <two>", xmlAfterTwo, printer4.CStr(), false);
}

void test44()
{
  // Crashing reported via email.
  const char *xml =
      "<playlist id='playlist1'>"
      "<property name='track_name'>voice</property>"
      "<property name='audio_track'>1</property>"
      "<entry out = '604' producer = '4_playlist1' in = '0' />"
      "<blank length = '1' />"
      "<entry out = '1625' producer = '3_playlist' in = '0' />"
      "<blank length = '2' />"
      "<entry out = '946' producer = '2_playlist1' in = '0' />"
      "<blank length = '1' />"
      "<entry out = '128' producer = '1_playlist1' in = '0' />"
      "</playlist>";

  // It's not a good idea to delete elements as you walk the
  // list. I'm not sure this technically should work; but it's
  // an interesting test case.
  XMLDocument doc;
  XMLError err = doc.Parse(xml);
  XMLTest("Crash bug parsing", XML_SUCCESS, err);

  XMLElement *playlist = doc.FirstChildElement("playlist");
  XMLTest("Crash bug parsing", true, playlist != 0);

  {
    const char *elementName = "entry";
    XMLElement *entry = playlist->FirstChildElement(elementName);
    XMLTest("Crash bug parsing", true, entry != 0);
    while (entry)
    {
      XMLElement *todelete = entry;
      entry = entry->NextSiblingElement(elementName);
      playlist->DeleteChild(todelete);
    }
    entry = playlist->FirstChildElement(elementName);
    XMLTest("Crash bug parsing", true, entry == 0);
  }
  {
    const char *elementName = "blank";
    XMLElement *blank = playlist->FirstChildElement(elementName);
    XMLTest("Crash bug parsing", true, blank != 0);
    while (blank)
    {
      XMLElement *todelete = blank;
      blank = blank->NextSiblingElement(elementName);
      playlist->DeleteChild(todelete);
    }
    XMLTest("Crash bug parsing", true, blank == 0);
  }

  tinyxml2::XMLPrinter printer;
  const bool acceptResult = playlist->Accept(&printer);
  XMLTest("Crash bug parsing - Accept()", true, acceptResult);
  printf("%s\n", printer.CStr());

  // No test; it only need to not crash.
  // Still, wrap it up with a sanity check
  int nProperty = 0;
  for (const XMLElement *p = playlist->FirstChildElement("property"); p; p = p->NextSiblingElement("property"))
  {
    nProperty++;
  }
  XMLTest("Crash bug parsing", 2, nProperty);
}

int main()
{
  // test35();
  // test36();
  // test37();
  // test38();
  // test39();
  // test40();
  // test41();
  // test42();
  // test43();
  test44();
}