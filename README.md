# 說明
這邊放的是我在大學時修過的課中寫過較大型的作業、project

## Git筆記
以下是我爬了各個git使用教學之後所整理出的筆記：

### 新增檔案/資料夾
要上傳到git之前，最好先將要新增的檔案移到和.git一樣的目錄，這樣子到時候上傳，目錄看起來比較整齊。

1. 打開「CMD」或「終端機」輸入`git add (檔案名稱)`  
(檔案名稱)：可以是一個檔案的名稱，或是一個資料夾的名稱。如果是資料夾的話，這樣上傳後會是整個資料夾的東西都會上傳。

2. 接著輸入`git commit -m "版本訊息"`  
"版本訊息"：這個是自訂的，上傳之後版本訊息會出現在目錄和更新時間的中間。

3. 最後輸入`git push origin main`  
不過我蠻常遇到push被拒絕的狀況，還好這個repository是給我自己用的，所以我可以每次都強制push。  
方法參考：https://gitbook.tw/chapters/github/fail-to-push

### 刪除資料夾
若是想要在遠端倉庫刪除特定資料夾，則：

1. `git rm -r --cached FOLDER_NAME` ：在cache中指定你要刪除的資料夾名稱
2. `git commit -m "YOUR_COMMENT"`：YOUR_COMMENT就是那個資料夾的版本訊息
3. `git push -u origin main`：推上倉庫
第三步我也是一樣用git push -f給它強制推