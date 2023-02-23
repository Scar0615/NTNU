import time
import warnings
from sklearn.metrics import accuracy_score
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier

start = time.time()
with open('./ML/allavg.csv') as csvfile:
    row=[]
    for lines in csvfile:                                 # 每次讀取一行
        tmp = lines.split(",")                            # 因為excel檔裡，兩個相鄰儲存格會用逗號隔開，所以用逗號區分資料。
        del tmp[-1]                                       # 最後一筆資料是時間，用不到所以刪除。
        row.append(tmp)                                   # 將這行讀到的資料放到row裡面
    del row[0]                                            # 第一筆資料只是編號，也用不到所以刪除。

    target = []
    data =[]
    for a in row:
        target.append(a[1])                               # target存放ID
        data.append(a[2:])                                # data存放其他資料

    dTrain, dTest, tTrain, tTest = train_test_split(data, target, test_size=0.2, random_state=0)

    for i in range(3,31,2):
        # Create a classifier: a support vector classifier
        classifier = KNeighborsClassifier(n_neighbors=i,weights='distance')

        # We learn the digits on the first half of the digits
        classifier.fit(dTrain, tTrain)

        # Now predict the value of the digit on the second half:
        expected = tTest                       # 訓練資料
        
        predicted = classifier.predict(dTest)  # 測試資料

        warnings.filterwarnings("ignore")
        
        # print("Classification report for classifier %s:\n%s\n"
        #     % (classifier, metrics.classification_report(expected, predicted)))
        # print("Confusion matrix:\n%s"
        #     % metrics.confusion_matrix(expected, predicted))

        print('k=%d'%i)
        print(accuracy_score(tTest, predicted))
        
        end = time.time()
        elapsed = end - start
        print("Time taken: ", elapsed, "seconds.")