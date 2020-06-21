import sys 
from PySide2.QtWidgets import (QApplication, QLabel, QPushButton,
                               QVBoxLayout, QWidget, QFileDialog,
                               QMainWindow)
from PySide2.QtCore import Slot, Qt, QFile, QIODevice
from PySide2.QtUiTools import QUiLoader
import sounddevice 
import csv, pathlib

SETTINGS_FILE = str(pathlib.Path(__file__).parent / "../../settings") #(move settings to shared module)
UI_FILE_NAME = "wavetable_gui.ui" # Qt Designer ui file TODO fix path!
WIDGETS_PAIRS = [('buffer_size', 'numframes'), ('sample_rate', 'samplerate'), ('memory_size', 'memsize')]
WIDGET_LOAD_SEPARATELY = [('output_devices', 'device')]

#TODO change these to shared module
def read_settings(path):
    settings_dict = {}
    with open(path) as csvfile:
        reader = csv.reader(csvfile)
        settings_dict = {row[0]:row[1][1:] for row in reader}
    return settings_dict 

def write_settings(settings_dict, path):
    with open(path, 'w') as csvfile:
        writer = csv.writer(csvfile)
        for value in settings_dict:
            writer.writerow([value, " "+ settings_dict[value]])

# TODO group objects that are settings control instead of hardcoded hack !!!
def change_enabled_settings(widgets, exceptions = ["file_chooser"], enable = True):
    for child in widgets.children():
        if type(child) != QLabel and child.objectName() not in exceptions:
            try: 
                child.setEnabled(enable)
            except:
                pass

class Interface(QMainWindow):

    def __init__(self, parent = None):
        QMainWindow.__init__(self)

        self.settings = read_settings(SETTINGS_FILE)

        self.load_view()
        self.load_output_devices(self.settings['device'])
        self.load_settings()

        self.central_widget.load_project.clicked.connect(self.choose_project)
        self.central_widget.run_button.clicked.connect(self.start_synth)

        self.setWindowTitle("Wavetable Synthesizer")
        self.setCentralWidget(self.central_widget)

        self.show()

    def start_synth(self):
        ### Chosen settings from user input
        for name, value in WIDGETS_PAIRS + WIDGET_LOAD_SEPARATELY:
            widget = getattr(self.central_widget, name)
            self.settings[value] = widget.currentText()
        write_settings(self.settings, SETTINGS_FILE)

    def choose_project(self):
        self.chosen_project, filter_type = QFileDialog.getOpenFileName(self.central_widget, 'Open file', filter = "DIA project (*.dia)")

        if self.chosen_project:
            self.central_widget.project_name.setText(pathlib.Path(self.chosen_project).stem)
            change_enabled_settings(self.central_widget, exceptions = ["load_project"])

            imported_settings = read_settings(self.chosen_project)
            self.settings = {**self.settings, **imported_settings} # merge setting dictionaries
            self.load_settings()

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

    def load_output_devices(self, default_device):
        output_devices = self.central_widget.output_devices
        for device in sounddevice.query_devices():
            if device['max_output_channels'] > 0:
                name = device['name']
                output_devices.addItem(name)
                if name == default_device:
                    output_devices.setCurrentIndex(output_devices.count()-1) # device index
                    
    def load_settings(self):
        for pair in WIDGETS_PAIRS:
            self.load_setting(*pair)

    def load_setting(self, widget_name, value):
        widget = getattr(self.central_widget, widget_name)

        if value not in self.settings: # instead of catching KeyErrors
            return

        default_value = self.settings[value]
        for index in range(widget.count()):
            if widget.itemText(index) == default_value:
                widget.setCurrentIndex(index)
                break
