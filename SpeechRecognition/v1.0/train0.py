# coding=utf-8
from __future__ import division, print_function, absolute_import
import tflearn
import speech_data3
import tensorflow as tf
import librosa
import numpy as np

learning_rate = 0.0001
training_iters = 500# steps
batch_size = 64

width = 20  # mfcc features
height = 80  # (max) length of utterance
classes = 10  # digits

load = word_batch = speech_data3.mfcc_batch_generator(2402) 
X, Y = next(load) # prend le prochain batch du générateur
trainX, trainY = X, Y
testX, testY = X, Y #overfit for now

# Network building
net = tflearn.input_data([None, width, height])
net = tflearn.lstm(net, 128, dropout=0.8)
net = tflearn.fully_connected(net, classes, activation='softmax')
net = tflearn.regression(net, optimizer='adam', learning_rate=learning_rate, loss='categorical_crossentropy')

 
i=0
model = tflearn.DNN(net, tensorboard_verbose=0)

model.fit(trainX, trainY, n_epoch=1000, show_metric=True,batch_size=64)

model.save("tflearn.lstm.model")
print(_y)
y,src = librosa.load("original.wav",mono = True)
mfcc=librosa.feature.mfcc(y,src)
MFCC=np.pad(mfcc,((0,0),(0,80-len(mfcc[0]))),mode='constant',constant_values=0)
print(model.predict([MFCC]))
""", validation_set=(testX, testY)"""
