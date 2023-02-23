# 利用 jieba 工具模組進行斷詞
#
# 目前沒有設定自用辭典（user diectionary）
# jieba.load_userdict(file_name)
# 特定專有術語應該放入此辭典中，如：CIS 180 image words
# 目前也沒有設定 stopwords
# 若除去英文（非中文）字元後，至少還應該除去全形標點符號
#

import jieba
import logging

def main():

    logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)
    
    # 支援繁體文句分詞的專用辭典
    jieba.set_dictionary('dict.txt.big')
    # 載入額外的使用者辭典
    # jieba.load_userdict('ccom223_iwdb.txt')

    # 輸出分詞結果
    ofile = open('wiki_seg.txt', 'w', encoding='utf-8')

    n = 0
    
    with open('wiki_zh_tw.txt' ,'r', encoding='utf-8') as ifile:
        for line in ifile:
            words = jieba.cut(line, cut_all=False)
            for w in words:
                ofile.write(w + ' ')
                n = n + 1
                if (n % 1000000 == 0):
                    # print('已完成前 %d 行的斷詞' % n)
                    logging.info('已完成前 %d 行的斷詞' % n)

    ifile.close()
    ofile.close()

if __name__ == '__main__':
    main()

