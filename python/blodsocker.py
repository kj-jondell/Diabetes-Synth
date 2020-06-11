### Interpreter of blood glucose data from Abbot FreeStyle Libre. Spline interpolation and ouptut of 1024 sample wav-file.
### TODO: 1. Cleanup code
### 2. automation (DONE)
###     b) automate extraction of glucose levels from excel-spreadsheet (or even API from freestyle libre app..)

from dateutil import parser
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import BSpline
from scipy import signal
import scipy
import soundfile as sf
import xlrd

# SETTINGS
SAMPLE_RATE = 48000
BUFFER_SIZE = 2048
WINDOW_SIZE = 0.1
IS_WAVETABLE = False #Supercollider wavetable format
WRITE_FILE = True
AMT_OUTPUT = 30

# CALIBRATION
ROW_OFFSET = 5
SHEET_NO = 1

#function taken from https://stackoverflow.com/questions/54032515/spectral-centroid-of-numpy-array
def spectral_centroid(x, samplerate=44100):
    magnitudes = np.abs(np.fft.rfft(x))
    length = len(x)
    freqs = np.abs(np.fft.fftfreq(length, 1.0/samplerate)[:length//2+1])
    magnitudes = magnitudes[:length//2+1]
    return np.sum(magnitudes*freqs) / np.sum(magnitudes)

times = []
values = []
window = signal.tukey(BUFFER_SIZE, WINDOW_SIZE) #window function, to smoothen buffer

centroids = list()
sheet = xlrd.open_workbook("blodsocker.xls").sheet_by_index(SHEET_NO) #TODO filechooser to select xls file...

amtData = sheet.nrows - ROW_OFFSET
baseTime = 0
for row in range(amtData):
    ### read and interpret data.
    dateString = sheet.cell_value(rowx = ROW_OFFSET + row, colx = 0)
    glucoseLevel = sheet.cell_value(rowx = ROW_OFFSET + row, colx = 1)

    date = parser.parse(dateString)
    values.append(glucoseLevel)

    if baseTime == 0:
        baseTime = date
    times.append((int)((date-baseTime).total_seconds()/60))

spl = BSpline(times, values, k=1)

### output extraction
for sampleIndex in range(0, AMT_OUTPUT):
    sample = []
    for x in range(BUFFER_SIZE*sampleIndex,BUFFER_SIZE*(sampleIndex+1)):
        sample.append(spl(x))
    sample = [(x-min(sample)) for x in sample]
    sample = [ 2*(x/(max(sample)-min(sample))-0.5) for x in sample] #normalize and center sound

    for ind, w_sample in enumerate(window):
        sample[ind] = sample[ind]*w_sample
    if IS_WAVETABLE:
        for ind, c_sample in enumerate(sample):
            if ind % 2 == 0:
                sample[ind] = 2*sample[ind] - sample[ind+1]
            else:
                sample[ind] = sample[ind] - sample[ind-1]

    centroids.append(spectral_centroid(sample, SAMPLE_RATE)) #prints spectral centroids (TODO: print as list)
    if WRITE_FILE:
        sf.write('blodsocker{}.wav'.format(sampleIndex+1), sample, SAMPLE_RATE)

print(centroids)
