from lxml import etree

# 加载XML文件
tree = etree.parse('data.xml')
root = tree.getroot()

# 遍历每个<site>元素
for site in root.iter('site'):
    code = site.get('code')
    name_en = site.findtext('nameEn')
    name_fr = site.findtext('nameFr')
    province_code = site.findtext('provinceCode')

    # 输出解析结果
    print('Site Code:', code)
    print('Name (EN):', name_en)
    print('Name (FR):', name_fr)
    print('Province Code:', province_code)

