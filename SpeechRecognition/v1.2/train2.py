#!/usr/bin/env python
#!/usr/bin/env PYTHONIOENCODING="utf-8" python
import tflearn
#import pyaudio
import speech_data
import numpy
import sys

load=speech_data.wave_batch_generator(2404)
X,Y=next(load)

number_classes=10 # Digits

tflearn.init_graph(num_cores=8, gpu_memory_fraction=0.5) 
# nombre de coeur allouer et memoire gpu allouer

net = tflearn.input_data(shape=[None, 8192])
net = tflearn.fully_connected(net, 64)
net = tflearn.dropout(net, 0.5)
net = tflearn.fully_connected(net, number_classes, activation='softmax')
net = tflearn.regression(net, optimizer='adam', learning_rate=0.0001 ,loss='categorical_crossentropy')

model = tflearn.DNN(net)
model.fit(X, Y,validation_set=0.2,n_epoch=200,show_metric=True,snapshot_step=100)

model.save("lstm")
demo_file = sys.argv[1]
demo=speech_data.load_wav_file(demo_file)
result=model.predict([demo])
result=numpy.argmax(result)
print("predicted digit for %s : result = %d "%(demo_file,result))
