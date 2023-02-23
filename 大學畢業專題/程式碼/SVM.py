import time
import warnings
from sklearn import svm
from sklearn import metrics
from sklearn.metrics import accuracy_score
from sklearn.model_selection import train_test_split

with open('./MLp/allavg.csv') as csvfile:    #檔案名
    row=[]
    for lines in csvfile:
        tmp = lines.split(",")
        del tmp[-1] # 去除"time"欄位
        row.append(tmp)
    del row[0]  # 去除"NO"欄位

    target = []
    data =[]
    for a in row:
        target.append(a[1]) # "ID"欄位
        data.append(a[2:])  # "LERI", "FREQ", "順序", "fsr1", ..., "fsr40"欄位

    mode = ['rbf', 'linear', 'poly', 'sigmoid']
    for i in mode:
        for j in range(1):
            start = time.time()
            dTrain, dTest, tTrain, tTest = train_test_split(data, target, test_size=0.2, random_state=0)

            # Create a classifier: a support vector classifier
            # classifier = svm.SVC(C=1.0, kernel='linear', degree=3, gamma='scale', coef0=0.0, shrinking=True, probability=False, tol=0.001, cache_size=200, class_weight=None, verbose=False, max_iter=-1, decision_function_shape='ovr', random_state=None)
            classifier = svm.SVC(kernel=i)

            # We learn the digits on the first half of the digits
            classifier.fit(dTrain, tTrain)

            # Now predict the value of the digit on the second half:
            expected = tTest
            
            predicted = classifier.predict(dTest)

            warnings.filterwarnings("ignore")
            
            # print("Classification report for classifier %s:\n%s\n"
            #     % (classifier, metrics.classification_report(expected, predicted)))
            # print("Confusion matrix:\n%s"
            #     % metrics.confusion_matrix(expected, predicted))

            end = time.time()
            elapsed = end - start
            print('mode:',i,'acc:',accuracy_score(tTest, predicted),"Time taken:", elapsed, "seconds.")