from dataclasses import dataclass
from dateutil import parser
import numpy 
from scipy.interpolate import BSpline
from scipy import signal
import scipy
import soundfile as sf
import xlrd
import View

### MODEL
# SETTINGS
SAMPLE_RATE = 48000
BUFFER_SIZE = 2048
WINDOW_SIZE = 0.1
IS_WAVETABLE = False #Supercollider wavetable format
WRITE_FILE = True
WINDOW_TYPE = "Hann"
AMT_OUTPUT = 3
OUTPUT_FILENAME = ""

# CALIBRATION
ROW_OFFSET = 5
SHEET_NO = 1
WINDOWS = {"Tukey" : signal.tukey, "Hamming" : signal.hamming, "Hann" : signal.hann, "Boxcar" : signal.boxcar, "Blackman" : signal.blackman} # TODO fix parameters for each type of window

@dataclass
class Model:
    filename: str
    view: View
    sample_rate: int = SAMPLE_RATE
    buffer_size: int = BUFFER_SIZE
    window_size: float = WINDOW_SIZE
    is_wavetable: bool = IS_WAVETABLE
    write_file: bool = WRITE_FILE
    window_type: str = WINDOW_TYPE
    amt_output: int = AMT_OUTPUT
    output_filename: str = OUTPUT_FILENAME 

    def __post_init__(self):
        self.window = WINDOWS[self.window_type](self.buffer_size, self.window_size if self.window_type == "Tukey" else None) #window function, to smoothen buffer TODO make selectable

        self.centroids = list()
        self.progress = 5
        if self.view.update_progressbar(self.progress, "Opening datafile"):
            return

        self.sheet = xlrd.open_workbook(self.filename).sheet_by_index(SHEET_NO) 
        self.progress += 5
        if self.view.update_progressbar(self.progress, "Parsing data"):
            return


        self.times, self.values = self.parse_data(self.sheet)
        self.progress += 5
        if self.view.update_progressbar(self.progress, "Creating spline"):
            return

        self.spl = BSpline(self.times, self.values, k = 1)
        self.progress += 5
        if self.view.update_progressbar(self.progress, "Extracting output"):
            return

        self.extract_output(self.spl)
        self.progress = 100
        self.view.update_progressbar(self.progress, "Done")

    #function taken from https://stackoverflow.com/questions/54032515/spectral-centroid-of-numpy-array
    def spectral_centroid(self, x, samplerate = 44100):
        magnitudes = numpy.abs(numpy.fft.rfft(x))
        length = len(x)
        freqs = numpy.abs(numpy.fft.fftfreq(length, 1.0/samplerate)[:length//2+1])
        magnitudes = magnitudes[:length//2+1]
        return numpy.sum(magnitudes*freqs) / numpy.sum(magnitudes)

    def parse_data(self, sheet):
        amt_data = sheet.nrows - ROW_OFFSET
        base_time = 0
        times = []
        values = []
        for row in range(amt_data):
            ### read and interpret data.
            date_string = sheet.cell_value(rowx = ROW_OFFSET + row, colx = 0)
            glucose_level = sheet.cell_value(rowx = ROW_OFFSET + row, colx = 1)

            date = parser.parse(date_string)
            values.append(glucose_level)

            if base_time == 0:
                base_time = date
            times.append((int)((date-base_time).total_seconds()/60))
        return times, values

    ### output extraction
    def extract_output(self, spl):
        progress_division = (100-self.progress)/(self.amt_output*3) 
        for sample_index in range(0, self.amt_output):
            self.progress += progress_division 
            if self.view.update_progressbar(self.progress, "Normalizing and centering data"):
                return

            sample = []
            for x in range(self.buffer_size*sample_index, self.buffer_size*(sample_index+1)):
                sample.append(spl(x))
            sample = [(x-min(sample)) for x in sample]
            sample = [ 2*(x/(max(sample)-min(sample))-0.5) for x in sample] #normalize and center sound

            self.progress += progress_division 
            if self.view.update_progressbar(self.progress, "Apply window{}".format(" and wavetable" if self.is_wavetable else "")):
                return

            for ind, w_sample in enumerate(self.window):
                sample[ind] = sample[ind]*w_sample
            if self.is_wavetable:
                for ind, c_sample in enumerate(sample):
                    if ind % 2 == 0:
                        sample[ind] = 2*sample[ind] - sample[ind+1]
                    else:
                        sample[ind] = sample[ind] - sample[ind-1]

            self.progress += progress_division 
            if self.view.update_progressbar(self.progress, "Calculate centroid{}".format(" and write files" if self.write_file else "")):
                return

            self.centroids.append(self.spectral_centroid(sample, self.sample_rate)) #prints spectral centroids (TODO: print as list)
            if self.write_file:
                sf.write(self.output_filename.format(sample_index+1), sample, self.sample_rate) #ask for name
