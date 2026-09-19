import time
import warnings
from sklearn.metrics import accuracy_score
from sklearn.model_selection import train_test_split
from sklearn.ensemble import GradientBoostingClassifier

# allavg: 一開始的平均資料
with open('./excel/allavg.csv') as csv_one:
    # 資料前處理
    row=[]
    for lines in csv_one:
        tmp = lines.split(",")
        del tmp[0]
        row.append(tmp)
    del row[0]

    # ID當作目標、其他資料當作數據
    target = []
    data =[]
    for a in row:
        target.append(a[0])
        data.append(a[3:])

    # 取每一位每一隻腳的前16次當訓練集，剩下的4次當測試集。
    train_target = []
    train_data = []
    test_target = []
    test_data = []
    for i in range(len(target)):
        if i%20 < 16:
            train_target.append(target[i])
            train_data.append(data[i])
        else:
            test_target.append(target[i])
            test_data.append(data[i])

    # 訓練並辨識
    start = time.time()
    clf = GradientBoostingClassifier()
    clf.fit(train_data, train_target)
    predicted = clf.predict(test_data)
    warnings.filterwarnings("ignore")

    end = time.time()
    elapsed = end - start
    
    print('allavg - acc:',accuracy_score(test_target, predicted),"Time taken:", elapsed, "seconds.")

# max: 最大壓力值
with open('./excel/max.csv') as csv_two:
    # 資料前處理
    row=[]
    for lines in csv_two:
        tmp = lines.split(",")
        del tmp[0]
        row.append(tmp)
    del row[0]

    # ID當作目標、其他資料當作數據
    target = []
    data =[]
    for a in row:
        target.append(a[0])
        data.append(a[3:])

    # 取每一位每一隻腳的前16次當訓練集，剩下的4次當測試集。
    train_target = []
    train_data = []
    test_target = []
    test_data = []
    for i in range(len(target)):
        if i%20 < 16:
            train_target.append(target[i])
            train_data.append(data[i])
        else:
            test_target.append(target[i])
            test_data.append(data[i])

    # 訓練並辨識
    start = time.time()
    clf = GradientBoostingClassifier()
    clf.fit(train_data, train_target)
    predicted = clf.predict(test_data)
    warnings.filterwarnings("ignore")

    end = time.time()
    elapsed = end - start

    print('max - acc:',accuracy_score(test_target, predicted),"Time taken:", elapsed, "seconds.")

# avg: 平均壓力值
with open('./excel/avg.csv') as csv_three:
    # 資料前處理
    row=[]
    for lines in csv_three:
        tmp = lines.split(",")
        del tmp[0]
        row.append(tmp)
    del row[0]

    # ID當作目標、其他資料當作數據
    target = []
    data =[]
    for a in row:
        target.append(a[0])
        data.append(a[3:])

    # 取每一位每一隻腳的前16次當訓練集，剩下的4次當測試集。
    train_target = []
    train_data = []
    test_target = []
    test_data = []
    for i in range(len(target)):
        if i%20 < 16:
            train_target.append(target[i])
            train_data.append(data[i])
        else:
            test_target.append(target[i])
            test_data.append(data[i])

    # 訓練並辨識
    start = time.time()
    clf = GradientBoostingClassifier()
    clf.fit(train_data, train_target)
    predicted = clf.predict(test_data)
    warnings.filterwarnings("ignore")

    end = time.time()
    elapsed = end - start
    
    print('avg - acc:',accuracy_score(test_target, predicted),"Time taken:", elapsed, "seconds.")

# allavgmax: 一開始的平均資料+最大壓力值
with open('./excel/allavgmax.csv') as csv_four:
    # 資料前處理
    row=[]
    for lines in csv_four:
        tmp = lines.split(",")
        del tmp[0]
        row.append(tmp)
    del row[0]

    # ID當作目標、其他資料當作數據
    target = []
    data =[]
    for a in row:
        target.append(a[0])
        data.append(a[3:])

    # 取每一位每一隻腳的前16次當訓練集，剩下的4次當測試集。
    train_target = []
    train_data = []
    test_target = []
    test_data = []
    for i in range(len(target)):
        if i%20 < 16:
            train_target.append(target[i])
            train_data.append(data[i])
        else:
            test_target.append(target[i])
            test_data.append(data[i])

    # 訓練並辨識
    start = time.time()
    clf = GradientBoostingClassifier()
    clf.fit(train_data, train_target)
    predicted = clf.predict(test_data)
    warnings.filterwarnings("ignore")

    end = time.time()
    elapsed = end - start

    print('allavgmax - acc:',accuracy_score(test_target, predicted),"Time taken:", elapsed, "seconds.")

# allavgavg: 一開始的平均資料
with open('./excel/allavgavg.csv') as csv_five:
    # 資料前處理
    row=[]
    for lines in csv_five:
        tmp = lines.split(",")
        del tmp[0]
        row.append(tmp)
    del row[0]

    # ID當作目標、其他資料當作數據
    target = []
    data =[]
    for a in row:
        target.append(a[0])
        data.append(a[3:])

    # 取每一位每一隻腳的前16次當訓練集，剩下的4次當測試集。
    train_target = []
    train_data = []
    test_target = []
    test_data = []
    for i in range(len(target)):
        if i%20 < 16:
            train_target.append(target[i])
            train_data.append(data[i])
        else:
            test_target.append(target[i])
            test_data.append(data[i])

    # 訓練並辨識
    start = time.time()
    clf = GradientBoostingClassifier()
    clf.fit(train_data, train_target)
    predicted = clf.predict(test_data)
    warnings.filterwarnings("ignore")

    end = time.time()
    elapsed = end - start
    
    print('allavgavg - acc:',accuracy_score(test_target, predicted),"Time taken:", elapsed, "seconds.")

# max: 一開始的平均資料
with open('./excel/aama.csv') as csv_six:
    # 資料前處理
    row=[]
    for lines in csv_six:
        tmp = lines.split(",")
        del tmp[0]
        row.append(tmp)
    del row[0]

    # ID當作目標、其他資料當作數據
    target = []
    data =[]
    for a in row:
        target.append(a[0])
        data.append(a[3:])

    # 取每一位每一隻腳的前16次當訓練集，剩下的4次當測試集。
    train_target = []
    train_data = []
    test_target = []
    test_data = []
    for i in range(len(target)):
        if i%20 < 16:
            train_target.append(target[i])
            train_data.append(data[i])
        else:
            test_target.append(target[i])
            test_data.append(data[i])

    # 訓練並辨識
    start = time.time()
    clf = GradientBoostingClassifier()
    clf.fit(train_data, train_target)
    predicted = clf.predict(test_data)
    warnings.filterwarnings("ignore")

    end = time.time()
    elapsed = end - start

    print('aama - acc:',accuracy_score(test_target, predicted),"Time taken:", elapsed, "seconds.")

# avglw: 一開始的平均資料+長寬
with open('./excel/avglw.csv') as csv_seven:
    # 資料前處理
    row=[]
    for lines in csv_seven:
        tmp = lines.split(",")
        del tmp[0]
        row.append(tmp)

    # ID當作目標、其他資料當作數據
    train_target = []
    train_data =[]
    for a in row:
        train_target.append(a[0])
        train_data.append(a[3:])

    # 取每一位每一隻腳的前16次當訓練集，剩下的4次當測試集。
    train_target = []
    train_data = []
    test_target = []
    test_data = []
    lt = len(target)
    for i in range(lt):
        if i%20 < 16:
            train_target.append(target[i])
            train_data.append(data[i])
        else:
            test_target.append(target[i])
            test_data.append(data[i])

    # 訓練並辨識
    start = time.time()
    clf = GradientBoostingClassifier()
    clf.fit(train_data, train_target)
    predicted = clf.predict(test_data)
    warnings.filterwarnings("ignore")

    end = time.time()
    elapsed = end - start
    
    print('avglw - acc:',accuracy_score(test_target, predicted),"Time taken:", elapsed, "seconds.")

# avgcg: 一開始的平均資料+重心
with open('./excel/avgcg.csv') as csv_eight:
    # 資料前處理
    row=[]
    for lines in csv_eight:
        tmp = lines.split(",")
        del tmp[0]
        row.append(tmp)

    # ID當作目標、其他資料當作數據
    target = []
    data =[]
    for a in row:
        target.append(a[0])
        data.append(a[3:])

    # 取每一位每一隻腳的前16次當訓練集，剩下的4次當測試集。
    train_target = []
    train_data = []
    test_target = []
    test_data = []
    lt = len(target)
    for i in range(lt):
        if i%20 < 16:
            train_target.append(target[i])
            train_data.append(data[i])
        else:
            test_target.append(target[i])
            test_data.append(data[i])

    # 訓練並辨識
    start = time.time()
    clf = GradientBoostingClassifier()
    clf.fit(train_data, train_target)
    predicted = clf.predict(test_data)
    warnings.filterwarnings("ignore")

    end = time.time()
    elapsed = end - start

    print('avgcg - acc:',accuracy_score(test_target, predicted),"Time taken:", elapsed, "seconds.")

# avglwcg: 一開始的平均資料+長寬+重心
with open('./excel/avglwcg.csv') as csv_nine:
    # 資料前處理
    row=[]
    for lines in csv_nine:
        tmp = lines.split(",")
        del tmp[0]
        row.append(tmp)
    del row[0]

    # ID當作目標、其他資料當作數據
    target = []
    data =[]
    for a in row:
        target.append(a[0])
        data.append(a[3:])

    # 取每一位每一隻腳的前16次當訓練集，剩下的4次當測試集。
    train_target = []
    train_data = []
    test_target = []
    test_data = []
    lt = len(target)
    for i in range(lt):
        if i%20 < 16:
            train_target.append(target[i])
            train_data.append(data[i])
        else:
            test_target.append(target[i])
            test_data.append(data[i])

    # 訓練並辨識
    start = time.time()
    clf = GradientBoostingClassifier()
    clf.fit(train_data, train_target)
    predicted = clf.predict(test_data)
    warnings.filterwarnings("ignore")

    end = time.time()
    elapsed = end - start

    print('avglwcg - acc:',accuracy_score(test_target, predicted),"Time taken:", elapsed, "seconds.")

# aamalc: 一開始的平均資料+最大壓力+平均壓力+長寬+重心
with open('./excel/aamalc.csv') as csv_ten:
    # 資料前處理
    row=[]
    for lines in csv_ten:
        tmp = lines.split(",")
        del tmp[0]
        row.append(tmp)
    del row[0]

    # ID當作目標、其他資料當作數據
    target = []
    data =[]
    for a in row:
        target.append(a[0])
        data.append(a[3:])

    # 取每一位每一隻腳的前16次當訓練集，剩下的4次當測試集。
    train_target = []
    train_data = []
    test_target = []
    test_data = []
    lt = len(target)
    for i in range(lt):
        if i%20 < 16:
            train_target.append(target[i])
            train_data.append(data[i])
        else:
            test_target.append(target[i])
            test_data.append(data[i])

    # 訓練並辨識
    start = time.time()
    clf = GradientBoostingClassifier()
    clf.fit(train_data, train_target)
    predicted = clf.predict(test_data)
    warnings.filterwarnings("ignore")

    end = time.time()
    elapsed = end - start

    print('aamalc - acc:',accuracy_score(test_target, predicted),"Time taken:", elapsed, "seconds.")