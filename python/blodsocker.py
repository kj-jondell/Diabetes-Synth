### Interpreter of blood glucose data from Abbot FreeStyle Libre. Spline interpolation and ouptut of 1024 sample wav-file.
### TODO: 1. Cleanup code
### 2. automation
### 3. improve date interpolation (include year for example)

from dateutil import parser
import numpy as np
import matplotlib.pyplot as plt
from scipy import interpolate
import soundfile as sf

SAMPLE_RATE = 48000
BUFFER_SIZE = 1024

times = []
values = []
with open('python/blodsocker.txt', 'r') as reader:
    ### read and interpret data.
    lines = reader.readlines()
    amtData = len(lines)
    for line in lines:
        splitline = line.split()
        date = parser.parse(splitline[0] + " " + splitline[1])
        values.append(float(splitline[2]))
        times.append(date.month*60*744+date.day*60*24+date.hour*60+date.minute)

    ### interpolation
    times = [int(x - times[0]) for x in times]
    tck = interpolate.splrep(times[0:BUFFER_SIZE], values[0:BUFFER_SIZE], k=1)

    ### output extraction
    amtOuptut = 15
    for sampleIndex in range(0,amtOuptut):
        sample = []
        for x in range(BUFFER_SIZE*sampleIndex,BUFFER_SIZE*(sampleIndex+1)):
            sample.append(interpolate.splev(x,tck))
        sample = [(x-min(sample)) for x in sample]
        sample = [ 2*(x/max(sample)-0.5) for x in sample] #normalize and center sound

        sf.write('samples/blodsocker{}.wav'.format(sampleIndex+1), sample, SAMPLE_RATE)


