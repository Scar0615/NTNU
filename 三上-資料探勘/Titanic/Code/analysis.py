import numpy as np
import pandas as pd

# 讀入資料
train = pd.read_csv('/Users/erichsieh/Desktop/NTNU/三上/資料探勘/V_40847016S/titanic/train.csv')
test = pd.read_csv('/Users/erichsieh/Desktop/NTNU/三上/資料探勘/V_40847016S/titanic/test.csv')
gender = pd.read_csv('/Users/erichsieh/Desktop/NTNU/三上/資料探勘/V_40847016S/titanic/gender_submission.csv')

dfTrain = pd.DataFrame(train)
dfTest = pd.DataFrame(test)
dfGender = pd.DataFrame(gender)

# 不同性別的生存率
women = dfTrain[dfTrain["Sex"] == 'female']
wsr = sum(women["Survived"])/len(women["Survived"])
print('女性乘客存活率為%.2f%%'%(wsr*100))

men = dfTrain[dfTrain["Sex"] == 'male']
msr = sum(men["Survived"])/len(men["Survived"])
print('男性乘客存活率為%.2f%%'%(msr*100))

# 不同社會經濟地位的生存率
for i in range(1,4):
    pclass = dfTrain[dfTrain["Pclass"] == i]
    psr = sum(pclass["Survived"])/len(pclass["Survived"])
    print('第%d級社會經濟地位乘客存活率為%.2f%%'%(i,psr*100))

# 年齡的部分某些為空，所以將年齡為空的那列做標記。
## 產生要標記的資料的索引串列
nanAge = []
dfTrainCheck = dfTrain.isnull()
for i in dfTrainCheck.index:
    if dfTrainCheck["Age"][i] == True:
        nanAge.append(i)

# 不同年齡的生存率
## 先對年齡做好分層處理
ageLoc = [0]
rec = 0
lt = len(dfTrain)
for i in range(10,90,10):
    tmp = 0
    for j in range(lt):
        if j in nanAge:
            continue
        if dfTrain["Age"][j]>i-10 and dfTrain["Age"][j]<=i:
            tmp += 1
    rec += tmp
    ageLoc.append(rec)

## 排序年齡
ageDfTrain = dfTrain.sort_values(["Age"], ascending=True)

## 統計生存率
lal = len(ageLoc)
for i in range(1,lal):
    age = ageDfTrain[ageDfTrain["Age"] <= i*10]
    partialAge = age[ageLoc[i-1]:ageLoc[i]]
    asr = sum(partialAge["Survived"])/len(partialAge["Survived"])
    print('%d~%d歲為%.2f%%'%((i-1)*10,i*10,asr*100))

# 不同上船港口的生存率
cherbourg = dfTrain[dfTrain["Embarked"] == 'C']
csr = sum(cherbourg["Survived"])/len(cherbourg["Survived"])
print('C港口共%d乘客上船，其乘客存活率為%.2f%%'%(len(cherbourg["Survived"]),csr*100))

queenstown = dfTrain[dfTrain["Embarked"] == 'Q']
qsr = sum(queenstown["Survived"])/len(queenstown["Survived"])
print('Q港口共%d乘客上船，其乘客存活率為%.2f%%'%(len(queenstown["Survived"]),qsr*100))

southampton = dfTrain[dfTrain["Embarked"] == 'S']
ssr = sum(southampton["Survived"])/len(southampton["Survived"])
print('S港口共%d乘客上船，其乘客存活率為%.2f%%'%(len(southampton["Survived"]),ssr*100))

# 不同票價的生存率
## 先對票價做好分層處理
fareItv = [-1,10,25,50,100,513]
fareLoc = [0]
lfi = len(fareItv)
rec = 0
for i in range(lfi-1):
    tmp = 0
    for j in range(lt):
        if dfTrain["Fare"][j]>fareItv[i] and dfTrain["Fare"][j]<=fareItv[i+1]:
            tmp += 1
    rec += tmp
    fareLoc.append(rec)
print(fareLoc)

## 排序票價
fareDfTrain = dfTrain.sort_values(["Fare"], ascending=True)

## 統計生存率
for i in range(1,lfi):
    fare = fareDfTrain[fareDfTrain["Fare"] <= fareItv[i]]
    partialFare = fare[fareLoc[i-1]:fareLoc[i]]
    fsr = sum(partialFare["Survived"])/len(partialFare["Survived"])
    print('%d~%d元為%.2f%%'%(fareItv[i-1],fareItv[i],fsr*100))