#!/usr/bin/env python

import ricaudio
from sepel.inputs import pyricaudio
import pylab
import sys
import scipy

filename = sys.argv[1]

samplerate = 44100

frameSize = 1024 
frameStep = 256

frameSizeTime = frameSize / 44100.0
frameStepTime = frameStep / 44100.0

fftSize = 2048

analysisLimit = 1000.0

# Creation of the pipeline        
stream = pyricaudio.sndfilereader({'filename': filename,
                                   'windowSizeInTime': frameSizeTime,
                                   'windowStepInTime': frameStepTime,
                                   'encodingKey': 'encoding',
                                   'channelCountKey': 'channelCount',
                                   'samplesOriginalKey': 'samples',
                                   'samplesKey': 'samplesMono',
                                   'samplerateKey': 'samplerate',
                                   'timestampBeginKey': 'timestampBegin',
                                   'timestampEndKey': 'timestampEnd',
                                   'limit':analysisLimit})


stream = pyricaudio.window_ricaudio(stream, {'inputKey': 'samplesMono',
                                             'outputKey': 'windowed',
                                             'windowType': 'hamming'})

stream = pyricaudio.fft_ricaudio(stream, {'inputKey': 'windowed',
                                          'outputKey': 'fft',
                                          'zeroPhase': True,
                                          'fftLength': fftSize})


odfNames = [(getattr(ricaudio.ODF, i), i) for i in dir(ricaudio.ODF)
            if type(getattr(ricaudio.ODF, i)) == int]

odfNames.sort()

specs = []
ffts = []
odfs = {}

for frame in stream:
    fft = scipy.array(frame['fft'][:fftSize/2], dtype = scipy.complex64)
    mag =  scipy.array(abs(fft), dtype = 'f4')
    spec =  20.0 / scipy.log( 10.0 ) * scipy.log( abs( fft ) + 1e-7)

    ffts.append( fft )
    specs.append( spec )

ffts = scipy.array( ffts )
specs = scipy.array( specs )

subplots = len(odfNames) + 1

pylab.figure()
pylab.hold(True)
pylab.subplot(subplots, 1, 1)
pylab.imshow( scipy.flipud(specs.T), aspect = 'auto' )
pylab.title( 'Spectrogram' )
ax = pylab.gca()
ticks = ax.get_xticks()
ax.set_xticklabels(['%.2f' % (float(tick) * frameStep / samplerate) for tick in ticks])

ax.set_yticks([])
ax.set_yticklabels([])
    
# Create the ODF processors and process
for i, (odfType, odfName) in enumerate(odfNames):
    odf = ricaudio.ODF( fftSize, odfType )
    odfValues = odf.process( ffts )

    pylab.subplot(subplots, 1, i+2)
    pylab.plot(odfValues[:,0])
    pylab.title( odfName.replace('_', ' ').capitalize() )
    ax = pylab.gca()
    
    ax.set_xticks([])
    ax.set_yticks([])
    
    ax.set_xticklabels([])
    ax.set_yticklabels([])
        
pylab.show()