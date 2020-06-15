import sys 
from PySide2.QtWidgets import (QApplication, QLabel, QPushButton,
                               QVBoxLayout, QWidget, QFileDialog,
                               QMainWindow)
from PySide2.QtCore import Slot, Qt, QFile, QIODevice
from PySide2.QtUiTools import QUiLoader
import sounddevice 

UI_FILE_NAME = "wavetable_gui.ui" # Qt Designer ui file TODO fix path!

class Interface(QMainWindow):

    def __init__(self, parent = None):
        QMainWindow.__init__(self)

        self.load_view()
        self.load_output_devices()

        self.setWindowTitle("Wavetable Synthesizer")
        self.setCentralWidget(self.central_widget)
        self.show()

    def load_view(self):
        self.ui_file = QFile(UI_FILE_NAME)
        if not self.ui_file.open(QIODevice.ReadOnly):
            print("Cannot open {}: {}".format(UI_FILE_NAME, self.ui_file.errorString()))
            sys.exit(-1)

        self.loader = QUiLoader()
        self.central_widget = self.loader.load(self.ui_file)
        self.ui_file.close()

        if not self.central_widget:
            print(self.loader.errorString())
            sys.exit(-1)

    #TODO read from settings file
    def load_output_devices(self, default_device = "Soundflower (2ch)"):
        output_devices = self.central_widget.output_devices
        for device in sounddevice.query_devices():
            if device['max_output_channels'] > 0:
                name = device['name']
                output_devices.addItem(name)
                if name == default_device:
                    output_devices.setCurrentIndex(output_devices.count()-1) # device index
                    

