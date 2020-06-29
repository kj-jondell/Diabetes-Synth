import csv, sounddevice
from PySide2.QtWidgets import QLabel, QWidget
from pathlib import Path

SETTINGS_FILE = str(Path(__file__).parent / "../../settings") #(move settings to shared module)

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
def read_settings(path = SETTINGS_FILE):
    settings_dict = {}
    try:
        with open(path) as csvfile:
            reader = csv.reader(csvfile)
            settings_dict = {row[0]:row[1][1:] for row in reader}
    except FileNotFoundError:
        pass # ignore missing settings file!
    return settings_dict 

def write_settings(settings_dict, path = SETTINGS_FILE):
    with open(path, 'w') as csvfile:
        writer = csv.writer(csvfile)
        for value in settings_dict:
            writer.writerow([value, " "+ settings_dict[value]])

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

