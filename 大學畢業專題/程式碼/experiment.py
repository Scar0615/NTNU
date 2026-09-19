"""腳底壓力辨識:重現專題報告第四章的實驗。

用法(在專案資料夾內):
    pip install -r requirements.txt
    python 程式碼/experiment.py

切法(報告 4.1.3):每位受試者、每隻腳,前 16 次走過的資料訓練,後 4 次測試。
特徵:只用感測器的壓力值(fsr1~fsr40),再依實驗加上重心(gx, gy)、長寬(len, wid)。
答案(受試者編號 ID)絕不能放進特徵。
"""
import time
import warnings
from pathlib import Path

import pandas as pd
from sklearn import svm
from sklearn.ensemble import GradientBoostingClassifier, RandomForestClassifier
from sklearn.neighbors import KNeighborsClassifier
from xgboost import XGBClassifier

warnings.filterwarnings("ignore")

DATA = Path(__file__).resolve().parent.parent / "資料集"
FSR = [f"fsr{i}" for i in range(1, 41)]
SEEDS = [0, 1, 2]  # 隨機演算法各跑 3 次取平均(報告也是取 3 次平均)

# 演算法名稱 -> 用 seed 建立模型
MODELS = {
    "SVM": lambda s: svm.SVC(kernel="linear"),
    "RF": lambda s: RandomForestClassifier(random_state=s),
    "GB": lambda s: GradientBoostingClassifier(random_state=s),
    "kNN": lambda s: KNeighborsClassifier(),
    "XGBoost": lambda s: XGBClassifier(random_state=s),
}


def split(df, step_col):
    """依「第幾次走過」切成訓練(1~16)與測試(17~20)。"""
    return df[df[step_col] <= 16], df[df[step_col] > 16]


def evaluate(train, test, id_col, features, models, seeds):
    """回傳 {模型: (平均準確度 %, 平均訓練秒數)}。"""
    ytr, yte = train[id_col].values - 1, test[id_col].values - 1  # XGBoost 要求標籤從 0 開始
    result = {}
    for name in models:
        accs, secs = [], []
        for seed in seeds:
            model = MODELS[name](seed)
            start = time.time()
            model.fit(train[features].values, ytr)
            secs.append(time.time() - start)
            accs.append((model.predict(test[features].values) == yte).mean() * 100)
        result[name] = (sum(accs) / len(accs), sum(secs) / len(secs))
    return result


def show(title, result):
    print(f"\n### {title}")
    print("| 演算法 | 準確度 | 訓練時間 |")
    print("|---|---|---|")
    for name, (acc, sec) in result.items():
        print(f"| {name} | {acc:.1f}% | {sec:.3f} 秒 |")


def main():
    # 平均資料:每次走過的 25 幀取平均,共 600 筆(15 人 × 左右腳 × 20 次)
    for csv, extra, title in [
        ("allavg.csv", [], "平均資料"),
        ("avggr.csv", ["gx", "gy"], "平均資料 + 重心"),
        ("avglwgr.csv", ["len", "wid", "gx", "gy"], "平均資料 + 重心 + 長寬"),
    ]:
        df = pd.read_csv(DATA / csv)
        train, test = split(df, "Order")
        show(f"{title}(訓練 {len(train)} 筆 / 測試 {len(test)} 筆)",
             evaluate(train, test, "ID", FSR + extra, MODELS, SEEDS))

    # 原始資料:每幀一筆,共 15000 筆(報告只比較表現最好的三種)
    raw = pd.read_csv(DATA / "allpre-25fps.csv")
    train, test = split(raw, "FREQ")
    show(f"原始資料(訓練 {len(train)} 筆 / 測試 {len(test)} 筆)",
         evaluate(train, test, "NUM", FSR, ["RF", "GB", "XGBoost"], SEEDS[:1]))

    # 對照:把答案(受試者編號)混進特徵會怎樣。當年的實驗就是這樣寫錯的,詳見 README「更正說明」
    df = pd.read_csv(DATA / "allavg.csv")
    train, test = split(df, "Order")
    show("對照:平均資料,特徵誤含 ID(標籤洩漏,結果不可信)",
         evaluate(train, test, "ID", FSR + ["ID"], ["RF", "GB", "XGBoost"], SEEDS))
    train, test = split(raw, "FREQ")
    show("對照:原始資料,特徵誤含 NUM(標籤洩漏,結果不可信)",
         evaluate(train, test, "NUM", FSR + ["NUM"], ["RF", "GB", "XGBoost"], SEEDS[:1]))


if __name__ == "__main__":
    main()
