### Interpreter of blood glucose data from Abbot FreeStyle Libre. Spline interpolation and ouptut of 1024 sample wav-file.
### TODO: 1. Cleanup code
### 2. automation (DONE)
###     b) automate extraction of glucose levels from excel-spreadsheet (or even API from freestyle libre app..)
### 3. improve date interpolation (include year for example)
### 4. sort by centroid frequency

from dateutil import parser
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import BSpline
from scipy import signal
import scipy
import soundfile as sf

SAMPLE_RATE = 48000
BUFFER_SIZE = 2048
IS_WAVETABLE = False #Supercollider wavetable format
WRITE_FILE = False
AMT_OUTPUT = 30

#function taken from https://stackoverflow.com/questions/54032515/spectral-centroid-of-numpy-array
def spectral_centroid(x, samplerate=44100):
    magnitudes = np.abs(np.fft.rfft(x))
    length = len(x)
    freqs = np.abs(np.fft.fftfreq(length, 1.0/samplerate)[:length//2+1])
    magnitudes = magnitudes[:length//2+1]
    return np.sum(magnitudes*freqs) / np.sum(magnitudes)

days_per_month = [31,28,31,30,31,30,31,31,30,31,30,31]
times = []
values = []
window = signal.tukey(BUFFER_SIZE, 0.1) #window function, to smoothen buffer
with open('python/blodsocker.txt', 'r', encoding='utf-8-sig') as reader:
    ### read and interpret data.
    lines = reader.readlines()
    amtData = len(lines)
    baseTime = 0
    for line in lines:
        splitline = line.split()
        date = parser.parse(splitline[0] + " " + splitline[1])
        values.append(float(splitline[2]))
        year = date.year-2019
        time = (year*365+sum(days_per_month[0:(date.month-1)])+date.day)*60*24+date.hour*60+date.minute
        if baseTime == 0:
            baseTime = time
        times.append(time-baseTime)

    spl = BSpline(times, values, k=1)

    ### output extraction
    for sampleIndex in range(0,AMT_OUTPUT):
        sample = []
        for x in range(BUFFER_SIZE*sampleIndex,BUFFER_SIZE*(sampleIndex+1)):
            sample.append(spl(x))
        sample = [(x-min(sample)) for x in sample]
        sample = [ 2*(x/max(sample)-0.5) for x in sample] #normalize and center sound

        for ind, w_sample in enumerate(window):
            sample[ind] = sample[ind]*w_sample
        if IS_WAVETABLE:
            for ind, c_sample in enumerate(sample):
                if ind % 2 == 0:
                    sample[ind] = 2*sample[ind] - sample[ind+1]
                else:
                    sample[ind] = sample[ind] - sample[ind-1]

        print(spectral_centroid(sample, SAMPLE_RATE)) #prints spectral centroids (TODO: print as list)
        if WRITE_FILE:
            sf.write('samples/wavetable2048/blodsocker{}.wav'.format(sampleIndex+1), sample, SAMPLE_RATE)


