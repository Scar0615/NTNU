import gensim
import re

# 定義函數：去除非中文的文字與符號

def remove_punctuation(line):
    rule = re.compile(r'[^\u4e00-\u9fa5|\s]')
    line = rule.sub('', line)
    return line

# 定義函數：去除多餘的空白符號

def remove_redundant_space(line):
    line = re.sub(' +', ' ', line)
    return line

# 下載的 wiki 語料檔
wiki_file = 'zhwiki-20190901-pages-articles.xml.bz2'

with open('wiki_texts.txt', 'w', encoding='utf8') as fp:
    # 利用 gensim 載入
    wiki = gensim.corpora.WikiCorpus(wiki_file, lemmatize=False, dictionary={})
    # 取出文字部分（原本是 XML 格式，包含很多標籤）
    for text in wiki.get_texts():
        # print(text)
        # text 是一篇文章，表示成字串串列（List）
        # text 中的字串連接合併成長字串，以空白字元作為間隔
        s = ' '.join(text)
        # 僅保留中文
        t = remove_punctuation(s)
        u = remove_redundant_space(t)
        # 每篇文章一個換行作為間隔，寫入輸出檔案
        fp.write(u + '\n')

fp.close()

