### Interpreter of blood glucose data from Abbot FreeStyle Libre. Spline interpolation and ouptut of 1024 sample wav-file.
### TODO: 1. Cleanup code
### 2. automation (DONE)
###     b) automate extraction of glucose levels from excel-spreadsheet (or even API from freestyle libre app..)
### 3. GUI! (MVC)
### 4. camelCase to snake_case...

from dateutil import parser
import numpy 
from scipy.interpolate import BSpline
from scipy import signal
import scipy
import soundfile as sf
import xlrd

### MODEL
# SETTINGS
SAMPLE_RATE = 48000
BUFFER_SIZE = 2048
WINDOW_SIZE = 0.1
IS_WAVETABLE = False #Supercollider wavetable format
WRITE_FILE = True
WINDOW_TYPE = "Boxcar"
AMT_OUTPUT = 30

# CALIBRATION
ROW_OFFSET = 5
SHEET_NO = 1
WINDOWS = {"Tukey" : signal.tukey, "Hamming" : signal.hamming, "Hann" : signal.hann, "Boxcar" : signal.boxcar, "Blackman" : signal.blackman} # TODO fix parameters for each type of window

class Model:

    def __init__(self, filename = "blodsocker.xls", sample_rate = SAMPLE_RATE, 
            buffer_size = BUFFER_SIZE, window_size = WINDOW_SIZE, 
            is_wavetable = IS_WAVETABLE, write_file = WRITE_FILE, 
            signal_type = WINDOW_TYPE, amt_output = AMT_OUTPUT):

        self.window = WINDOWS[WINDOW_TYPE](BUFFER_SIZE, WINDOW_SIZE if WINDOW_TYPE == "Tukey" else None) #window function, to smoothen buffer TODO make selectable
        self.times = []
        self.values = []

        self.centroids = list()
        self.sheet = xlrd.open_workbook(filename).sheet_by_index(SHEET_NO) 

    #function taken from https://stackoverflow.com/questions/54032515/spectral-centroid-of-numpy-array
    def spectral_centroid(self, x, samplerate = 44100):
        magnitudes = numpy.abs(numpy.fft.rfft(x))
        length = len(x)
        freqs = numpy.abs(numpy.fft.fftfreq(length, 1.0/samplerate)[:length//2+1])
        magnitudes = magnitudes[:length//2+1]
        return numpy.sum(magnitudes*freqs) / numpy.sum(magnitudes)


amt_files = sheet.nrows - ROW_OFFSET
    def get_times(amt_files, sheet):
        base_time = 0
        for row in range(amt_files):
            ### read and interpret data.
            date_string = sheet.cell_value(rowx = ROW_OFFSET + row, colx = 0)
            glucose_level = sheet.cell_value(rowx = ROW_OFFSET + row, colx = 1)

            date = parser.parse(date_string)
            values.append(glucose_level)

            if base_time == 0:
                base_time = date
            times.append((int)((date-base_time).total_seconds()/60))
    return times

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
