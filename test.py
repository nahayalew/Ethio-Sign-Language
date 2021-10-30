import serial
#from externals import joblib
import pickle
import numpy as np
import pandas as pd
import time
import pyttsx3
from sklearn.ensemble import RandomForestClassifier


def Voice(label):
    print("add sth here!!! ")

pklFileName = "Random_forest_classification.pkl"

with open (pklFileName,'rb') as file:
    Model1 = pickle.load(file)

label = Model1.predict(X)
print(label)
engine = pyttsx3.init()
if label == 0:
    engine.say("sost")
else:
    engine.say(" ")
engine.runAndWait()

_serial = serial.Serial(port = 'COM4',baudrate=9600)
_serial.close()
_serial.open()
ard_data = list()


while True:
    #TODO find way to split incoming data
    data = _serial.readline().split(",")
    while(data!=";"):
        ard_data.append(data)
    #TODO convert the incoming data to float, if not float
    X = pd.DataFrame(ard_data)
    label = Model1.predict(X)
    Voice(label)
    time.sleep(30)
