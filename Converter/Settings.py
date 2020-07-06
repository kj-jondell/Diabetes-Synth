import csv, sounddevice
from dataclasses import dataclass
from PySide2.QtWidgets import QLabel, QWidget
from pathlib import Path

AMT_CHANNELS = 2 #TODO variable...

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
