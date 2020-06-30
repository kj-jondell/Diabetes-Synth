import csv, sounddevice
from dataclasses import dataclass
from PySide2.QtWidgets import QLabel, QWidget
from pathlib import Path

AMT_CHANNELS = 2 #TODO variable...

### MIDI / SOUND OUTPUT SETTINGS
#TODO move to Settings class
def load_midi_ports(channels, amt_channels = 16):
    for channel in range(1,amt_channels+1):
        channels.addItem("{}".format(channel))

#TODO move to Settings class
def load_output_devices(device, ports, amt_channels = 2):
    ports.clear()
    amt_outputs = sounddevice.query_devices(device)['max_output_channels']
    amt_outputs = amt_outputs if amt_outputs > amt_channels else amt_channels
    for port in range(1, amt_outputs, amt_channels): # 2 because stereo (works best with ableton)?) TODO implement multichannel!
        ports.addItem("{}-{}". format(port, port+(amt_channels-1)))

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

@dataclass
class Settings():
    """ 
    This is a class for reading and writing settings. Basically a wrapper for a dictionary...
    """
    projectname: str=None
    device: str=None
    mididevice: int=None
    port: int=None
    midichannel: int=None
    numframes: int=None
    samplerate: int=None
    memsize: int=None
    filename: str=None
    order: str=None
    availableports: int=None

    SETTINGS_FILE = str(Path(__file__).parent / "../../settings")

    def __str__(self):
        return str(self.__dict__)

    def read_settings(self, path = None):
        path = path if path else Settings.SETTINGS_FILE
        try:
            with open(path) as csvfile:
                reader = csv.reader(csvfile)
                for row in reader:
                    setattr(self, row[0], row[1][1:])
            return True
        except FileNotFoundError:
            return False

    def write_settings(self, path = None):
        path = path if path else Settings.SETTINGS_FILE
        with open(path, 'w') as csvfile:
            writer = csv.writer(csvfile)
            for value in self.__dict__:
                if getattr(self, value): #only write non-none data
                    writer.writerow([value, " " + str(getattr(self, value))])

    def get_midi_device_index(self):
        return int(self.midichannel)-1 if self.midichannel else 0

    def get_ports_index(self, channels = AMT_CHANNELS):
        return (int(self.port.split('-')[0])-1)/channels if self.port else 0

    def merge_settings(self, path = None, settings = None):
        old_settings = Settings()
        old_settings.read_settings(path)
        self.__dict__ = {**self.__dict__, **{k:v for k,v in old_settings.__dict__.items() if v}} # retain current settings when conflict (removes None items)
