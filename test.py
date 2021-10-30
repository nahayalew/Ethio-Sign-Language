import serial
#from externals import joblib
import pickle
import numpy as np
import pandas as pd
import time
import pyttsx3
from sklearn.ensemble import RandomForestClassifier

#text to speech conversion
engine = pyttsx3.init()
def Voice(label):
    if label == '0':
        engine.say("zero")
    elif label == '1':
        engine.say("aand")
    elif label == '2':
        engine.say("hulet")
    elif label == 3:
        engine.say("sost")
    elif label == '4':
        engine.say("arat")
    elif label == '5':
        engine.say("amist")
    elif label == '6':
        engine.say("sidist")
    elif label == '7':
        engine.say("sebat")
    elif label == '8':
        engine.say("simint")
    elif label == '9':
        engine.say("zetegn")
    elif label == '10':
        engine.say("ha")
    elif label == '11':
        engine.say("re")
    elif label == '12':
        engine.say("se")
    elif label == '13':
        engine.say("shee")
    elif label == '14':
        engine.say("be")
    elif label == '15':
        engine.say("gne")
    elif label == '16':
        engine.say("ke")
    elif label == '17':
        engine.say("aa")
    elif label == '18':
        engine.say("ze")
    elif label == '19':
        engine.say("fe")
    elif label == '20':
        engine.say("pe")
    else:
        engine.say("gesture not recognized")
    engine.runAndWait()

#load model
pklFileName = "Random_forest_classification.pkl"

with open (pklFileName,'rb') as file:
    Model1 = pickle.load(file)

_serial = serial.Serial(port = 'COM4',baudrate=9600)
_serial.close()
_serial.open()
ard_data = []
ttt = []

#receive incoming flex sensor data and perform classification
while True:
    retrieved_data = _serial.readline()#.decode().rstrip().split(",")
    retVal = retrieved_data.strip().decode()
    while (retVal != ";"):
        ard_data.append(float(retVal))
    ttt = [float(i) for i in ard_data]
    X = pd.DataFrame(ttt)#
    print(X)
    label = Model1.predict(X)
    print(label)
    Voice(label)
    ard_data.clear()
    time.sleep(3000)
