import time
import warnings
import pandas as pd
from sklearn import svm, metrics
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.neighbors import KNeighborsClassifier
from xgboost import XGBClassifier

avg = pd.read_csv('allavg.csv')
# avggr = pd.read_csv('avggr.csv')
# avglwgr = pd.read_csv('avglwgr.csv')

# 刪除不相關屬性(data: 訓練集, target: 測試集)
# # file avg
data = avg.drop(['NO','LERI','Order','time'],axis=1)
target = avg.ID

# # file avggr
# data = avggr.drop(['NO','LERI','Order'],axis=1)
# target = avggr.ID

# file avglwgr
# data = avglwgr.drop(['NO','LERI','Order'],axis=1)
# target = avglwgr.ID

leng = len(data)    # 600筆資料

# SVM classifier
svmc = svm.SVC(C=1.0, kernel='linear', degree=3, gamma='scale', coef0=0.0, shrinking=True, probability=False, tol=0.001, cache_size=200, class_weight=None, verbose=False, max_iter=-1, decision_function_shape='ovr', random_state=None)

# RF classifier
rfc = RandomForestClassifier()

# GB classifier
gbc = GradientBoostingClassifier()

# KNN classifier
knc = KNeighborsClassifier()

# XGBoost classifier
xgbc = XGBClassifier()

clist = [svmc, rfc, gbc, knc, xgbc]

# 訓練時間
for clf in clist:
    start = time.time()
    clf.fit(data[:int(leng*0.8)], target[:int(leng*0.8)])
    end = time.time()
    elapsed = end - start
    print("Time taken: ", elapsed, "seconds.")

    expected = target[int(leng*0.8):]
    predicted = clf.predict(data[int(leng*0.8):])

    warnings.filterwarnings("ignore")

    print("Classification report for classifier %s:\nAccuracy: %s\n" % (clf, metrics.accuracy_score(expected, predicted)))
    print("Confusion matrix:\n%s" % metrics.confusion_matrix(expected, predicted))

# avggr
# svmc: 0.73, rfc: 0.983, gbc: 0.975, knc: 0.542, xgbc: 1.0
# svmc: 0.73, rfc: 0.958, gbc: 0.975, knc: 0.542, xgbc: 1.0
# svmc: 0.73, rfc: 0.983, gbc: 0.975, knc: 0.542, xgbc: 1.0
# seconds avg
# svmc: 0.73, rfc: 0.975, gbc: 0.975, knc: 0.542, xgbc: 1.0

# avglwgr
# svmc: 0.73, rfc: 0.983, gbc: 0.975, knc: 0.542, xgbc: 1.0
# svmc: 0.73, rfc: 0.975, gbc: 0.975, knc: 0.542, xgbc: 1.0
# svmc: 0.73, rfc: 0.983, gbc: 0.975, knc: 0.542, xgbc: 1.0
# seconds avg
# svmc: 0.73, rfc: 0.978, gbc: 0.975, knc: 0.542, xgbc: 1.0