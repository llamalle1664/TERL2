# coding=utf-8
from __future__ import division, print_function, absolute_import
import tflearn
import speech_data3
import tensorflow as tf
import librosa
import numpy as np

learning_rate = 0.0001

width = 20
height = 80  
classes = 10  

batch = word_batch = speech_data3.mfcc_batch_generator(18000) #cree un generateur comprenant les 18000 fichiers ainsi que les labels
X, Y = next(batch) # prend le prochain batch du generateur
trainX, trainY = X, Y
testX, testY = X, Y

# Creation du réseau
net = tflearn.input_data([None, width, height])
net = tflearn.lstm(net, 128, dropout=0.8)
net = tflearn.fully_connected(net, classes, activation='softmax')
net = tflearn.regression(net, optimizer='adam', learning_rate=learning_rate, loss='categorical_crossentropy')


model = tflearn.DNN(net, tensorboard_verbose=0,checkpoint_path="tflearn.lstm.model")

#entrainement
model.fit(trainX, trainY, n_epoch=1000, show_metric=True,batch_size=64,snapshot_step=5000)

model.save("tflearn.lstm.model")
""", validation_set=(testX, testY)"""
