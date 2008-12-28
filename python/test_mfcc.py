#!/usr/bin/env python

# Create input
import scipy
lowFreq = 300.0
highFreq = 16000.0
nBands = 40
samplerate = 44100
spectralLength = 1024
nCoeffs = 13

minSpectrum = 1e-10
power = 1

a1 = scipy.array(scipy.zeros((1, 512)), dtype='f4')
a2 = scipy.array(scipy.ones((1, 512)), dtype='f4')



# CRicaudio's solution # --------------------------------- #
import ricaudio
m = ricaudio.MFCC(lowFreq, highFreq, nBands, samplerate, spectralLength, nCoeffs, minSpectrum, power)

b1 = m.process(a1).T
b2 = m.process(a2).T
# -------------------------------------------------------- #


print b1
print b2