# coding=utf-8
import os
import re
import sys
import wave

import numpy
import numpy as np
import skimage.io  # scikit-image

from random import shuffle

SOURCE_URL = 'http://pannous.net/files/' #spoken_numbers.tar'
DATA_DIR = '../data/'
pcm_path = "../data/spoken_numbers_pcm/" # 8 bit
wav_path = "../data/spoken_numbers_wav/" # 16 bit s16le
path = pcm_path
CHUNK = 4096
test_fraction=0.1 # 10% of data for test / verification

"""j'irais bien niquer ta mere"""
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

# If you set dynamic_pad=True when calling tf.train.batch the returned batch will be automatically padded with 0s. Handy! A lower-level option is to use tf.PaddingFIFOQueue.
# only apply to a subset of all images at one time
def wave_batch_generator(batch_size=10): #speaker
  batch_waves = []
  labels = []
  # input_width=CHUNK*6 # wow, big!!
  files = os.listdir(path)
  while True:
    shuffle(files)
    print("loaded batch of %d files" % len(files))
    for wav in files:
      if not wav.endswith(".wav"):continue
      labels.append(dense_to_one_hot(int(wav.split('_')[0])))
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


