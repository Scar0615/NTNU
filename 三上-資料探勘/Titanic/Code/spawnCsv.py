import numpy as np
import pandas as pd
import warnings
from sklearn.ensemble import RandomForestClassifier

# 讀入資料
train = pd.read_csv('/Users/erichsieh/Desktop/NTNU/三上/資料探勘/V_40847016S/titanic/train.csv')
test = pd.read_csv('/Users/erichsieh/Desktop/NTNU/三上/資料探勘/V_40847016S/titanic/test.csv')
gender = pd.read_csv('/Users/erichsieh/Desktop/NTNU/三上/資料探勘/V_40847016S/titanic/gender_submission.csv')

dfTrain = pd.DataFrame(train)
dfTest = pd.DataFrame(test)
dfGender = pd.DataFrame(gender)

# 對年齡空值填入中位數
trainAgeMedian = np.nanmedian(dfTrain['Age'])
newAge = np.where(dfTrain['Age'].isnull(), trainAgeMedian, dfTrain['Age'])
dfTrain['Age'] = newAge
print(dfTrain['Fare'].isnull().sum())

testAgeMedian = np.nanmedian(dfTest['Age'])
newAge = np.where(dfTest['Age'].isnull(), testAgeMedian, dfTest['Age'])
dfTest['Age'] = newAge
print(dfTest['Fare'].isnull().sum())

# 先單純對性別進行訓練測試
## 將性別量化
dfTrain['SexNum'] = dfTrain['Sex'].map({'female':0,'male':1}).astype('int')
dfTest['SexNum'] = dfTest['Sex'].map({'female':0,'male':1}).astype('int')

## 測試集有一個運費為空，填中位數。
testFareMedian = np.nanmedian(dfTest['Fare'])
newFare = np.where(dfTest['Fare'].isnull(), testFareMedian, dfTest['Fare'])
dfTest['Fare'] = newFare

## 乘客的生死是我們的target，所以將其他方面的資料當作data。
attributeList = ['Fare']
data = dfTrain[attributeList]
target = dfTrain['Survived']

clf = RandomForestClassifier(random_state=1,n_estimators=250,oob_score=True)
clf.fit(data, target)
predict = clf.predict(dfTest[attributeList])

warnings.filterwarnings('ignore')

output = pd.DataFrame({'PassengerId': dfTest.PassengerId, 'Survived': predict})
output.to_csv('f_result.csv', index=False)