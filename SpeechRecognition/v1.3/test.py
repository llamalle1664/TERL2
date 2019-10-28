#!/usr/bin/env python
#!/usr/bin/env PYTHONIOENCODING="utf-8" python
import tflearn
#import pyaudio
import speech_data
import numpy
import sys


number_classes=10 

mots=["dog","house","on","stop","sheila","marvin","wow","left","cat","go"]

net = tflearn.input_data(shape=[None, 8192])
net = tflearn.fully_connected(net, 64)
net = tflearn.dropout(net, 0.5)
net = tflearn.fully_connected(net, number_classes, activation='softmax')
net = tflearn.regression(net, optimizer='adam', learning_rate=0.001 ,loss='categorical_crossentropy')

model = tflearn.DNN(net)
model.load("tflearn.lstm.model")

demo_file = sys.argv[1]
demo=speech_data.load_wav_file(demo_file)
result=model.predict([demo])
result=numpy.argmax(result)
print("le mot du fichier est "+mots[result])
