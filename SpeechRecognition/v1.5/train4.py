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

load = word_batch = speech_data3.mfcc_batch_generator(20000,numClass=10) #créé un générateur comprenant les 18000 fichiers ainsi que les labels
X, Y = next(load) # prend le prochain batch du générateur
trainX, trainY = X, Y
testX, testY = X, Y #overfit for now

# Network building
net = tflearn.input_data([None, width, height])
net = tflearn.conv_1d(net,128,3)
net = tflearn.fully_connected(net, classes, activation='softmax')
net = tflearn.regression(net, optimizer='adam', learning_rate=learning_rate, loss='categorical_crossentropy')

i=0
model = tflearn.DNN(net, tensorboard_verbose=0,checkpoint_path="tflearn.lstm.model")

model.fit(trainX, trainY, n_epoch=1000, show_metric=True,batch_size=64,snapshot_step=500)

model.save("tflearn.lstm.model")
""", validation_set=(testX, testY)"""
