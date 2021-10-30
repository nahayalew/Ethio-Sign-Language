import pandas as pd
import numpy as np
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import PolynomialFeatures
from sklearn.svm import LinearSVC
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.externals import joblib
from sklearn.metrics import accuracy_score,precision_score,recall_score,f1_score
from sklearn.svm import SVC
from sklearn.metrics import confusion_matrix,plot_confusion_matrix,precision_recall_fscore_support 
import matplotlib.pyplot as plt
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score,precision_score,recall_score,f1_score

#load dataset
df = pd.read_csv('dataset1.csv')
df = df.drop(columns = 'Unnamed: 0')#drop index

X = df.drop(columns='label')
y = df['label']

#split dataset to train and test datasets
x_train, x_test, y_train, y_test = train_test_split(X,y,test_size = 0.3,random_state=42)#(X, y, test_size=0.3, random_state=0)
print("X_train.shape:",x_train.shape)
print("X_test.shape:",x_test.shape)
print("y_train.shape:",y_train.shape)
print("y_test.shape:",y_test.shape)

#train Linear SVM  
polynomial_svm__clf = Pipeline((
    ("polynomial_features",PolynomialFeatures(degree = 3)),
    ("scaler",StandardScaler()),
    ("svm_clf",LinearSVC(C=10,loss="hinge"))
))
polynomial_svm__clf.fit(x_train,y_train)
y_pred_poly = polynomial_svm__clf.predict(x_test)



#train SVM with polynomial kernel 
poly_kernel_svm_clf = Pipeline((
    ("scaler",StandardScaler()),
    ("svm_clf",SVC(kernel="poly",degree=3,coef0=1,C=5))#kernel uses 3rd degree polynomial
))
poly_kernel_svm_clf.fit(X,y)
y_pred_poly_kernel = poly_kernel_svm_clf.predict(x_test)

# train SVM with gaussian rbf kernel
rbf_kernel_svm_clf = Pipeline((
        ("scaler", StandardScaler()),
        ("svm_clf", SVC(kernel="rbf", gamma=10, C=10))
))
rbf_kernel_svm_clf.fit(x_train, y_train)
y_pred_gaussian_rbf = rbf_kernel_svm_clf.predict(x_test)


#train random forest classifier
rnd_clf = RandomForestClassifier(n_estimators=1000, max_leaf_nodes=64, n_jobs=-1)
rnd_clf.fit(x_train, y_train)
y_pred_rf = rnd_clf.predict(x_test)

#measure the test set performance
print("Accuracy:",accuracy_score(y_pred_poly_kernel,y_test)*100)
print("Precision:",precision_score(y_pred_poly_kernel,y_test,average = "macro")*100)
print("Recall:",recall_score(y_pred_poly_kernel,y_test,average = "macro")*100)
print("F1 score:",f1_score(y_pred_poly_kernel,y_test,average = "macro")*100)

print("Accuracy:",accuracy_score(y_pred_gaussian_rbf,y_test)*100)
print("Precision:",precision_score(y_pred_gaussian_rbf,y_test,average = "macro")*100)
print("Recall:",recall_score(y_pred_gaussian_rbf,y_test,average = "macro")*100)
print("F1 score:",f1_score(y_pred_gaussian_rbf,y_test,average = "macro")*100)

print("Accuracy:",accuracy_score(y_pred_rf,y_test)*100)
print("Precision:",precision_score(y_pred_rf,y_test,average = "macro")*100)
print("Recall:",recall_score(y_pred_rf,y_test,average = "macro")*100)
print("F1 score:",f1_score(y_pred_rf,y_test,average = "macro")*100)

print("Accuracy:",accuracy_score(y_pred_gaussian_rbf,y_test)*100)
print("Precision:",precision_score(y_pred_rf,y_test,average = "macro")*100)
print("Recall:",recall_score(y_pred_rf,y_test,average = "macro")*100)
print("F1 score:",f1_score(y_pred_rf,y_test,average = "macro")*100)

#confusion matrix to observe which letters are misclassified
yLab = y.unique()
cnf_matrix_poly = confusion_matrix(y_true=y_test, y_pred=y_pred_poly, labels=yLab)
plot_confusion_matrix(polynomial_svm__clf,x_test,y_test)
#plt.figure(figsize=[500000,500000])
plt.rcParams["figure.figsize"] = (200,10)
plt.show()

cnf_matrix_poly_kernel = confusion_matrix(y_true=y_test, y_pred=y_pred_poly_kernel, labels=yLab)
plot_confusion_matrix(poly_kernel_svm_clf,x_test,y_test)
#plt.figure(figsize=[500000,500000])
plt.rcParams["figure.figsize"] = (200,10)
plt.show()


cnf_matrix_poly_kernel = confusion_matrix(y_true=y_test, y_pred=y_pred_gaussian_rbf, labels=yLab)
plot_confusion_matrix(rbf_kernel_svm_clf,x_test,y_test)
#plt.figure(figsize=[500000,500000])
plt.rcParams["figure.figsize"] = (200,10)
plt.show()

cnf_matrix_randomF = confusion_matrix(y_true=y_test, y_pred=y_pred_rf, labels=yLab)
plot_confusion_matrix(rnd_clf,x_test,y_test)
#plt.figure(figsize=[500000,500000])
plt.rcParams["figure.figsize"] = (200,10)
plt.show()

# export the trained model
Model1 = polynomial_svm__clf # svm with Ploynomial feature
Model2 = poly_kernel_svm_clf #svm with polynomial kernel
Model3 = rbf_kernel_svm_clf #svm with gaussian kernel

joblib.dump(Model1,"../SVM_poly_features_multiclass_classification.pkl")
joblib.dump(Model2,"../SVM_poly_kernel_multiclass_classification.pkl")
joblib.dump(Model3,"../SVM_gaussian_kernel_multiclass_classification.pkl")

