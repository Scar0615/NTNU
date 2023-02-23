import time
import warnings
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression

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

    fte = targetTurnToFloat(tTest)     # Float Target tEst

    warnings.filterwarnings("ignore")
    
    # LinearRegression
    start = time.time()
    lr = LinearRegression()
    lr.fit(dTrain, tTrain)
    lrp = lr.predict(dTest)    # lr predicted
    end = time.time()
    elapsed = end - start
    print('Used function: LinearRegression')
    print("Time taken: ", elapsed, "seconds.")
    print('Different score:',difference(lrp, tTest))

    # 畫出tTest
    plt.plot(fte,'k')
    plt.plot(lrp,'r')
    plt.ylabel('Price')
    plt.show()