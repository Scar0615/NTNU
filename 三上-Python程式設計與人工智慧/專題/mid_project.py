from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time

# 前置作業
key = input('請輸入你要找的關鍵字：')
scrollTime = int(input('請輸入你要頁面下拉的次數：'))
path = "/usr/local/bin/chromedriver"
s = Service(path)
driver = webdriver.Chrome(service=s)

# 進入dcard
url = "https://www.dcard.tw/f"
driver.get(url)

time.sleep(1)

# 搜尋
search = driver.find_element_by_name('query') # dcard的搜尋欄的標籤為name='query'
search.clear()                                # 清空搜尋欄
search.send_keys(key)                         # 將要輸入的文字放這裡
search.send_keys(Keys.RETURN)                 # 按下enter鍵

# 等資料載入
WebDriverWait(driver, 10).until(
    EC.presence_of_element_located((By.CLASS_NAME, "sc-3yr054-1"))
)

# 統計文章數量用
cnt = 0
titleCheck = []
theme = {}

for i in range(scrollTime):
    # 頁面下拉
    page = driver.find_element_by_tag_name('body')
    time.sleep(1)
    page.send_keys(Keys.PAGE_DOWN)
    time.sleep(1)

    # 抓取文章並分析
    article = driver.find_elements_by_class_name('tgn9uw-0')
    for t in article:
        tt = t.text
        tns = tt.replace(' ','')
        tr = tns.replace('\n',' ')
        tl = list(tr.split())

        # 將頭像字元移除
        if len(tl)==8:
            del tl[0]
        
        # 判斷文章是否出現過
        if tl[2] not in titleCheck:
            cnt += 1
            titleCheck.append(tl[2])
            print('==============================================')
            print('文章計數：'+str(cnt)+'\n主題：'+tl[0]+'\n標題：'+tl[2]+'\n表情數：'+tl[4]+' 留言數：'+tl[5])

            # 放入主題統計
            if theme.__contains__(tl[0]):
                theme[tl[0]] += 1
            else:
                theme[tl[0]] = 1

# 依文章數量排序主題
st = sorted(theme.items(), key=lambda x:x[1], reverse=True)
print('==============================================')
print('主題統計：')
for t,n in st:
    print(t+'共'+str(n)+'篇')

# 退出視窗
time.sleep(3)
driver.quit()