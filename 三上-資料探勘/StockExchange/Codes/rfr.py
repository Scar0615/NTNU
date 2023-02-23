import time
import warnings
import matplotlib.pyplot as plt
from sklearn.ensemble import RandomForestRegressor

def dataTurnToFloatArray(df,div):
    ld = len(df)
    fd = []
    for i in range(ld):
        fd.append( [float(df[i][0])/div, float(df[i][1])/div] )
    return fd

def targetTurnToFloat(df):
    ld = len(df)
    new = []
    for i in range(ld):
        new.append(float(df[i]))
    return new

def difference(pre, ans):
    diff, lp = 0, len(pre)
    for i in range(lp):
        diff += abs(float(pre[i]) - float(ans[i]))
    return diff

def reduction(l, div):
    ll = len(l)
    nl = []
    for i in range(ll):
        nl.append(l[i]*div)
    return nl

with open('indexTW.csv') as csvfile:
    row=[]
    for lines in csvfile:
        tmp = lines.split(",")
        # 判斷該筆資料是否有null
        if tmp[2] == 'null':
            continue
        else:
            row.append(tmp)
    del row[0]              # 將標籤移除

    target = []
    data =[]
    for a in row:
        target.append(a[5]) # Close
        data.append(a[3:5]) # High,Low
    
    leng = len(data)
    
    dTrain = data[:int(leng*0.8)]
    dTest = data[int(leng*0.8):]
    tTrain = target[:int(leng*0.8)]
    tTest = target[int(leng*0.8):]

    fdr = dataTurnToFloatArray(dTrain, 1)   # Float Data tRain
    fde = dataTurnToFloatArray(dTest, 2)   # Float Data tEst
    ftr = targetTurnToFloat(tTrain)         # Float Target tRain
    fte = targetTurnToFloat(tTest)          # Float Target tEst

    warnings.filterwarnings("ignore")

    # RandomForestRegressor(預設為100)
    start = time.time()
    rfr = RandomForestRegressor()
    rfr.fit(dTrain, tTrain)
    rfrp = rfr.predict(fde)  # rfr predicted
    rrfrp = reduction(rfrp, 2)  # reducted rfrp
    end = time.time()
    elapsed = end - start
    print('Used function: RandomForestRegressor')
    print("Time taken: ", elapsed, "seconds.")
    print('Different score:',difference(rrfrp, tTest))

    # 畫出RFR和正解比較圖
    plt.plot(fte,'k')
    plt.plot(rrfrp,'r')
    plt.title('RandomForestRegressor')
    plt.ylabel('Price')
    plt.show()