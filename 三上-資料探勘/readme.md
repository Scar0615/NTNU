# 資料探勘

這堂課的份量很扎實:每週有紙本作業,另有期中、期末考,期末還要完成兩個專題:一個是個人專題(Titanic),一個是三人一組的小組專題(各國股價指數)。這裡收錄兩個專題的程式與報告,以及 scikit-learn 的練習。

## Titanic 生存預測(個人專題)

[Kaggle 競賽](https://www.kaggle.com/competitions/titanic):依乘客資料預測是否在鐵達尼號沉船中存活。完整的分析與過程見 [`Titanic/report.docx`](Titanic/report.docx)。

**資料分析**([`analysis.py`](Titanic/Code/analysis.py))逐一統計各特徵的存活率:

- 性別差異最大:女性 74.2%、男性 18.9%。
- 艙等越高越容易存活:第 1 級 63.0%、第 2 級 47.3%、第 3 級 24.2%。
- 票價越高存活率越高:10 元以下 19.9%,100 元以上 73.6%。

**前處理:** 年齡有 177 筆空值,分析存活率時略過,訓練時改填中位數;登船港口缺 2 筆,填入人數最多的 Southampton;測試集的票價缺 1 筆,填中位數。

**模型:** `RandomForestClassifier`(訓練快、預設參數的準確度就不錯、對雜訊的容忍度高)。用不同的特徵組合各訓練一次並上傳 Kaggle,分數如下:

| 使用的特徵 | Kaggle 分數 |
|---|---|
| 性別 | **0.76555** |
| 性別 + 艙等 | **0.76555** |
| 票價 | 0.69138 |
| 艙等 | 0.67224 |
| 性別 + 艙等 + 年齡 | 0.64114 |
| 性別 + 年齡 | 0.61244 |

只用性別(或性別加艙等)的分數最高,加入年齡後反而下降。報告的推測是特徵變多、干擾也跟著變多。各組合的預測結果放在 [`Titanic/Datasets/`](Titanic/Datasets)。

```sh
cd Titanic/Code
python analysis.py     # 各特徵的存活率分析
python spawnCsv.py     # 訓練並產生預測檔(改 attributeList 即可換特徵)
```

## 各國股價指數預測(小組專題)

資料集:[Stock Exchange Data](https://www.kaggle.com/datasets/mattiuzc/stock-exchange-data)(Kaggle),這裡收錄我的個人報告與程式,使用其中的**台灣加權指數**(`indexTW.csv`)。詳見 [`StockExchange/個人報告.docx`](StockExchange/個人報告.docx)。

**目標:** 用當日的最高價與最低價,預測當日收盤價。前 80% 的資料訓練、後 20% 測試,並比較兩個模型:

| 模型 | 預測值與實際值的總差距(越小越好) |
|---|---|
| `LinearRegression`([`lr.py`](StockExchange/Codes/lr.py)) | 約 3.9 萬 |
| `RandomForestRegressor`,未調整資料 | 約 140 萬 |
| `RandomForestRegressor`,調整資料後([`rfr.py`](StockExchange/Codes/rfr.py)) | 約 4.6 萬 |

數字取自報告。我重新執行 `lr.py` 與 `rfr.py`,分別得到 39,280 與 46,137;未調整資料的版本沒有另外重跑。

**發現:** 隨機森林的預測值有上下限,無法超出訓練資料見過的範圍。測試期間的股價高過訓練期的最高價時,它只能預測那個上限,誤差就越拉越大。我先把輸入縮小 2 倍,讓測試資料落在訓練時見過的範圍內,預測後再乘回 2 倍,差距就降到與線性迴歸相近。結論是這類近似線性、又會超出歷史範圍的資料,`LinearRegression` 不必調整資料、訓練速度快、預測也準,是比較適合的選擇。

```sh
cd StockExchange/Datasets      # 程式會讀取目前資料夾的 indexTW.csv
python ../Codes/lr.py
python ../Codes/rfr.py
```

## scikit-learn 練習

- [`sklearn1212.ipynb`](sklearn1212.ipynb):操作記錄單。kNN、Naive Bayes、決策樹、SVM、MLP 的分類與迴歸用法,評估指標(混淆矩陣、F1、precision-recall curve、R²),以及 KFold、ShuffleSplit 等資料切分方式。
- [`sklearn0116.ipynb`](sklearn0116.ipynb):對鳶尾花資料做 K-Means(分成 2 到 6 群),用輪廓係數(silhouette score)比較分群的好壞。

## 環境

```sh
pip install numpy pandas scikit-learn matplotlib jupyter
```
