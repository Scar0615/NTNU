# 系統程式

期末專題:用 Python 寫一個 **SIC/XE 組譯器**。讀入 SIC/XE 組合語言原始碼,逐行計算每個標籤(label)的記憶體位址,輸出 **symbol table**。

## 怎麼運作

輸入是 [`fp.txt`](Final%20Project/fp.txt)(一段 SIC/XE 組合語言程式,每行是 `label  instruction  operand`)。組譯器逐行掃描,維護一個位置計數器(location counter):

- 遇到新的 label,就用目前的位置計數器記進 symbol table,輸出時依 label 出現的先後排序。
- 每一行指令依類型累加位置計數器:

| 類型 | 佔用空間 |
|---|---|
| 假指令 `START`、`END`、`BASE` | 0 |
| 2 byte 指令 `CLEAR`、`COMPR`、`TIXR` | 2 |
| extended format(指令前有 `+`,如 `+JSUB`) | 4 |
| `RESW n` | 3n |
| `RESB n` | n |
| `BYTE` | `C'EOF'` 為 3;其他常數(如 `X'F1'`)為 1 |
| 其他指令 | 3 |

輸出範例(位址為 16 進位):

```
SYMBOL TABLE
COPY    0000
FIRST   0000
ENDFIL  0006
EOF     0019
RETADR  001C
LENGTH  001F
BUFFER  0022
RDREC   1022
...
```

## 執行

```sh
cd "Final Project"
python3 fp.py
```

`fp.txt` 需和 `fp.py` 放在同一個資料夾。印出 symbol table 後按 Enter 離開。

## 實作心得

完整的說明見 [`report.pdf`](Final%20Project/report.pdf)。摘要:

- 只要一行指令有 label,那行的 label、instruction、operand 三個欄位就都存在,可以用欄位數判斷該行有沒有 label。
- 所有假指令的 operand 都是常數;`BYTE` 的常數可能是 1 byte 或 3 byte(如 `EOF`)。
- 當時另外用 PyInstaller 把程式打包成 Windows 執行檔繳交。這裡只保留 Python 原始碼。
