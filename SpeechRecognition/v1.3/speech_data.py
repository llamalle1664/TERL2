# coding=utf-8
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

DATA_DIR = '../2speech/'
pcm_path = "../2speech/" # 8 bit
path = pcm_path
CHUNK = 4096
test_fraction=0.1 # 10% of data for test / verification

def load_wav_file(name):
  f = wave.open(name, "rb")
  # print("loading %s"%name)
  chunk = []
  data0 = f.readframes(CHUNK)
  while data0:  # f.getnframes()
    # data=numpy.fromstring(data0, dtype='float32')
    # data = numpy.fromstring(data0, dtype='uint16')
    data = numpy.fromstring(data0, dtype='uint8')
    data = (data + 128) / 255.  # 0-1 for Better convergence
    # chunks.append(data)
    chunk.extend(data)
    data0 = f.readframes(CHUNK)
  # finally trim:
  chunk = chunk[0:CHUNK * 2]  # should be enough for now -> cut
  chunk.extend(numpy.zeros(CHUNK * 2 - len(chunk)))  # fill with padding 0's
  # print("%s loaded"%name)
  return chunk


def wave_batch_generator(batch_size=10 , numClass=10): #speaker
  batch_waves = []
  labels = []
  files = os.listdir(path)
  while True:
    shuffle(files)
    print("loaded batch of %d files" % len(files))
    for wav in files:
      if not wav.endswith(".wav"):continue
      labels.append(dense_to_one_hot(int(wav[0]), numClass))
      chunk = load_wav_file(path+wav)
      batch_waves.append(chunk)
      # batch_waves.append(chunks[input_width])
      if len(batch_waves) >= batch_size:
        yield batch_waves, labels
        batch_waves = []  # Reset for next batch
        labels = []


def dense_to_one_hot(labels_dense, num_classes=10):
  """Convert class labels from scalars to one-hot vectors."""
  return numpy.eye(num_classes)[labels_dense]



if __name__ == "__main__":
  print("downloading speech datasets")
  maybe_download( Source.DIGIT_SPECTROS)
  maybe_download( Source.DIGIT_WAVES)
  maybe_download( Source.NUMBER_IMAGES)
  maybe_download( Source.NUMBER_WAVES)
