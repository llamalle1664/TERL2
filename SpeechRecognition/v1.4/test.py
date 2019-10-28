import tflearn
import speech_data3
import librosa
import tensorflow as tf
import numpy as np
import sys

width = 20
height = 80 
classes = 10  

mots = ["dog","house","on","stop","sheila","marvin","wow","left","cat","go"]

#creation du reseau
net = tflearn.input_data([None, width, height])
net = tflearn.lstm(net, 128, dropout=0.8)
net = tflearn.fully_connected(net, classes, activation='softmax')
net = tflearn.regression(net, optimizer='adam', learning_rate=0.001, loss='categorical_crossentropy')


model = tflearn.DNN(net, tensorboard_verbose=0)

#chargement du modele entraine
model.load("tflearn.lstm.model")

#chargement du fichier test
y,src = librosa.load(sys.argv[1],mono = True)
mfcc=librosa.feature.mfcc(y,src)
print(len(mfcc[1])+10)
MFCC=np.pad(mfcc,((0,0),(0,80-len(mfcc[0]))),mode='constant',constant_values=0) #erreur 80-len(mfcc[0]) est negatif , a priori corrige en reduisant la taille dechantillon test

#lancement du test
print(model.predict([MFCC]))
result=model.predict([MFCC])
result=np.argmax(result)
print("le mot du fichier est "+mots[result])
