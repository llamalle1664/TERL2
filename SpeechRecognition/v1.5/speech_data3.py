
import os
import re
import sys
import wave

import numpy
import numpy as np
import skimage.io  # scikit-image
import librosa
import matplotlib

from random import shuffle
from six.moves import urllib
from six.moves import xrange  # pylint: disable=redefined-builtin

DATA_DIR = '../speech/'
pcm_path = "../speech/" # 8 bit
wav_path = "../speech/" # 16 bit s16le
path = pcm_path
CHUNK = 4096
test_fraction=0.1 # 10% of data for test / verification

def mfcc_batch_generator(batch_size=10,numClass=10):
  batch_features = []
  labels = []
  files = os.listdir(path)
  while True:
    print("loaded batch of %d files" % len(files))
    shuffle(files)
    i=0
    for wav in files:
      print(i)
      i=i+1
      if not wav.endswith(".wav"): continue
      wave, sr = librosa.load(path+wav, mono=True) 
      #analyse fichier audio et retourne amplitude et frequence 
      
      label=dense_to_one_hot(int(wav.split("_")[0]),numClass) 
      #label = tenseur correspondant au label du fichier audio
      
      labels.append(label) 
      # ajoute le label du fichier au tableau de label


      mfcc=librosa.feature.mfcc(wave,sr)  
      #renvoie un tableau representant le spctre d un son en fonction de son amplitude et de son echantillonage

      # print(np.array(mfcc).shape)
      if (len(mfcc[0]) > 80) : continue 
      mfcc=np.pad(mfcc,((0,0),(0,80-len(mfcc[0]))), mode='constant', constant_values=0) 
      # normaliser la taille des tableaux mfcc
      
      batch_features.append(np.array(mfcc)) 
      # ajoute le tableau mfcc correspondant au fichier audio dans le tableau general batch features
      
      if len(batch_features) >= batch_size: 
        # quand on a traiter et transformer batch-size fichier : 
        # print(np.array(batch_features).shape)
        # yield np.array(batch_features), labels
        yield batch_features, labels  
        # met dans le generateur le batch_features et les labels
        
        batch_features = []  
        # Reset for next batch
        
        labels = []



# If you set dynamic_pad=True when calling tf.train.batch the returned batch will be automatically padded with 0s. Handy! A lower-level option is to use tf.PaddingFIFOQueue.
# only apply to a subset of all images at one time


# multi-label

def dense_to_one_hot(labels_dense, num_classes=10):
  """Convert class labels from scalars to one-hot vectors."""
  return numpy.eye(num_classes)[labels_dense]





