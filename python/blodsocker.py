### Interpreter of blood glucose data from Abbot FreeStyle Libre. Spline interpolation and ouptut of 1024 sample wav-file.
### TODO: 1. Cleanup code
### 2. automation (DONE)
### 3. improve date interpolation (include year for example)

from dateutil import parser
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import BSpline
from scipy import signal
import scipy
import soundfile as sf

SAMPLE_RATE = 48000
BUFFER_SIZE = 2048
IS_WAVETABLE = True #Supercollider wavetable format
AMT_OUTPUT = 30

days_per_month = [31,28,31,30,31,30,31,31,30,31,30,31]
times = []
values = []
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
        window = signal.tukey(BUFFER_SIZE, 0.1) #window function, to smoothen buffer
        for ind, w_sample in enumerate(window):
            sample[ind] = sample[ind]*w_sample
        if IS_WAVETABLE:
            for ind, c_sample in enumerate(sample):
                if ind % 2 == 0:
                    sample[ind] = 2*sample[ind] - sample[ind+1]
                else:
                    sample[ind] = sample[ind] - sample[ind-1]
        sf.write('samples/wavetable2048/blodsocker{}.wav'.format(sampleIndex+1), sample, SAMPLE_RATE)


