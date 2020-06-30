import csv, sounddevice
from dataclasses import dataclass
from PySide2.QtWidgets import QLabel, QWidget
from pathlib import Path

AMT_CHANNELS = 2 #TODO variable...

### MIDI / SOUND OUTPUT SETTINGS
def load_midi_ports(channels, amt_channels = 16):
    for channel in range(1,amt_channels+1):
        channels.addItem("{}".format(channel))

def load_output_devices(device, ports, amt_channels = 2):
    ports.clear()
    amt_outputs = sounddevice.query_devices(device)['max_output_channels']
    amt_outputs = amt_outputs if amt_outputs > amt_channels else amt_channels
    for port in range(1, amt_outputs, amt_channels): # 2 because stereo (works best with ableton)?) TODO implement multichannel!
        ports.addItem("{}-{}". format(port, port+(amt_channels-1)))

### CSV READ / WRITE
# TODO make settings class!

# TODO group objects that are settings control instead of hardcoded hack !!!
### QT HELPER FUNCTIONS
def change_enabled_settings(widgets, exceptions = ["file_chooser"], enable = True):
    for child in widgets.children():
        if type(child) != QLabel and child.objectName() not in exceptions:
            try: 
                child.setEnabled(enable)
            except:
                pass

def list_to_string(input_list = []):
    return str(input_list)[1:-1].replace(',', '')

# TODO implement this instead of settings dict
@dataclass
class Settings():
    """ 
    This is a class for reading and writing settings. Basically a wrapper for a dictionary...
    """
    device: str=""
    mididevice: int=0
    port: int=0
    midichannel: int=0
    numframes: int=0
    samplerate: int=0
    memsize: int=0
    filename: str=""
    order: str=""

    ### SETTINGS KEYWORDS
    #(DEVICE, MIDIDEVICE, PORT, MIDICHANNEL) = ('device', 'mididevice', 'port', 'midichannel')
    #(NUMFRAMES, SAMPLERATE, MEMSIZE, FILENAME) = ('numframes', 'samplerate', 'memsize', 'filename')
    #(ORDER) = ('order')

    SETTINGS_FILE = str(Path(__file__).parent / "../../settings")

    def __init__(self, path = None, read_settings = True):
        if not path:
            self.path = SETTINGS_FILE 
        else:
            self.path = path

        if read_settings:
            self.read_settings()
        else:
            self.settings = {}
    
    def read_settings(self):
        self.settings = {}
        try:
            with open(self.path) as csvfile:
                reader = csv.reader(csvfile)
                for row in reader:
                    setattr(self, row[0], row[1][1:])
                #self.settings = {row[0]:row[1][1:] for row in reader}
        except FileNotFoundError:
            pass # ignore missing settings file!

    def write_settings(self):
        with open(self.path, 'w') as csvfile:
            writer = csv.writer(csvfile)
            for value in self.settings:
                writer.writerow([value, " " + getattr(self, value)])

    def get_midi_device_index(self):
        return int(self.midichannel)-1

    def get_ports_index(self, channels = AMT_CHANNELS):
        return (int(self.port.split('-')[0])-1)/channels

    def merge_settings(self, path = None, settings = None):
        old_settings = settings if settings else Settings(path, read_settings=True)
        self.__dict__ = {**self.__dict__, **old_settings.__dict__} # retain current settings when conflict

    def has(self, key):
        return key in self.__dict__

    #TODO REMOVE
    def get(self, key):
        return getattr(self, key)
        #return self.settings[key]

    #TODO REMOVE
    def set(self, key, value):
        setattr(self, key, value)
        #self.settings[key] = value

    def is_empty(self): 
        return not bool(self.settings)

    # ### GETTERS
    # @property
    # def mididevice(self):
    #     return self.settings[Settings.MIDIDEVICE]

    # @property
    # def port(self):
    #     return self.settings[Settings.PORT]

    # @property
    # def midichannel(self):
    #     return self.settings[Settings.MIDICHANNEL]

    # @property
    # def device(self):
    #     return self.settings[Settings.DEVICE]

    # @property
    # def numframes(self):
    #     return self.settings[Settings.NUMFRAMES]

    # @property
    # def samplerate(self):
    #     return self.settings[Settings.SAMPLERATE]

    # @property
    # def memsize(self):
    #     return self.settings[Settings.MEMSIZE]

    # @property
    # def filename(self):
    #     return self.settings[Settings.FILENAME]

    # @property
    # def order(self):
    #     return self.settings[Settings.ORDER]

    # ### SETTERS
    # @mididevice.setter
    # def mididevice(self, value):
    #     self.settings[Settings.MIDIDEVICE] = value

    # @port.setter
    # def port(self, value):
    #     self.settings[Settings.PORT] = value

    # @midichannel.setter
    # def midichannel(self, value):
    #     self.settings[Settings.MIDICHANNEL] = value

    # @device.setter
    # def device(self, value):
    #     self.settings[Settings.DEVICE] = value

    # @numframes.setter
    # def numframes(self, value):
    #     self.settings[Settings.NUMFRAMES] = value

    # @samplerate.setter
    # def samplerate(self, value):
    #     self.settings[Settings.SAMPLERATE] = value

    # @memsize.setter
    # def memsize(self, value):
    #     self.settings[Settings.MEMSIZE] = value

    # @filename.setter
    # def filename(self, value):
    #     self.settings[Settings.FILENAME] = value

    # @order.setter
    # def order(self, value):
    #     self.settings[Settings.ORDER] = value
