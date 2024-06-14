# 三方库
from lxml import etree  # 解析xml文件
import openpyxl  # 操作excel文件
from openpyxl import Workbook
import pandas as pd  # 操作excel文件

import codecs  # 编码相关


fileContext = [[]]  # 保存xml解析出来的文本


def parseXMLFile(name):
    # 加载XML文件
    tree = etree.parse(name)
    root = tree.getroot()

    # 遍历每个<site>元素
    for site in root.iter('site'):
        code = site.get('code')
        name_en = site.findtext('nameEn')
        name_fr = site.findtext('nameFr')
        province_code = site.findtext('provinceCode')

        curContext = []
        curContext.append(code)
        curContext.append(name_en)
        curContext.append(name_fr)
        curContext.append(province_code)
        fileContext.append(curContext)


def writeToFilePD(name):
    df = pd.DataFrame(fileContext)
    df.to_excel(name, index=False, header=False)
    print("Excel has been writed\n")


def writeToFile(name):
    wb = Workbook()
    ws = wb.active

    for row_idx, row_data in enumerate(fileContext, 1):
        for col_idx, cell_value in enumerate(row_data, 1):
            ws.cell(row=row_idx, column=col_idx, value=cell_value)
    wb.save(name)
    print("Excel has been writed\n")


if __name__ == "__main__":
    parseXMLFile("siteList.xml")
    # writeToFile("output.xlsx")
    writeToFilePD("output.xlsx")

