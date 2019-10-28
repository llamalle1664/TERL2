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

def mfcc_batch_generator(batch_size=10):
#permet de creer un tenseur contenant les informations du fichier audio et un tenseur de labels
  batch_features = []
  labels = []
  files = os.listdir(path)
  while True:
    print("loaded batch of %d files" % len(files))
    shuffle(files)
    for wav in files:
        if not wav.endswith(".wav"): continue
        wave, sr = librosa.load(path+wav, mono=True) 
      #analyse fichier audio et retourne amplitude et frequence 

        label=dense_to_one_hot(int(wav[0]),10) 
      #label = tenseur correspondant au label du fichier audio

        labels.append(label) 
      # ajoute le label du fichier au tableau de label


        mfcc=librosa.feature.mfcc(wave,sr)  
      #renvoie un tableau representant le spctre d un son en fonction de son amplitude et de son echantillonage

      # print(np.array(mfcc).shape)
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
        # libere le tableau

            labels = []


def dense_to_one_hot(labels_dense, num_classes=10):
    """Converti les labels en tableau de taille num-classes ou ce qui a dans la case numero labels_dense est 1 et le reste 0  """
    return numpy.eye(num_classes)[labels_dense]
