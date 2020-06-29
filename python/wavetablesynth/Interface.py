import sys, subprocess, rtmidi
from PySide2.QtWidgets import (QApplication, QLabel, QPushButton,
                               QVBoxLayout, QWidget, QFileDialog,
                               QMainWindow, QMenu, QMenuBar)
from PySide2.QtCore import Slot, Qt, QFile, QIODevice
from PySide2.QtUiTools import QUiLoader
from python.controller import Controller
import sounddevice 
import csv
from pathlib import Path
import python.helper.helper as helper

VERBOSITY_SETTING =  ['-v', '--verbose']

BREAK_COMMAND = "startup finished"
UI_FILE_NAME = str(Path(__file__).parent / "ui/wavetable_gui.ui") # Qt Designer ui file TODO fix path!
WIDGETS_PAIRS = [('buffer_size', 'numframes'), ('sample_rate', 'samplerate'), ('memory_size', 'memsize')]
WIDGET_LOAD_SEPARATELY = [('output_devices', 'device'), ('midi_devices', 'mididevice'), ('ports', 'port'), ('channels', 'midichannel')]

AMT_CHANNELS = 2 #TODO variable...

class Interface(QMainWindow):

    def __init__(self, args = None):
        QMainWindow.__init__(self)

        self.parse_arguments(args)

        self.settings = helper.read_settings(helper.SETTINGS_FILE)
        self.sc_process = None
        self.controller_window = None

        self.load_view()
        if self.settings:
            self.load_output_devices(self.settings['device'])
            self.load_midi_devices(self.settings['mididevice'])
            helper.load_midi_ports(self.central_widget.channels)
        self.load_settings()

        # helper.change_enabled_settings(self.central_widget, exceptions = ["open_controller", "load_project"]) # TODO TEMPORARY!!!! must be removed!

        self.central_widget.load_project.clicked.connect(self.choose_project)
        self.central_widget.run_button.clicked.connect(self.start_synth)
        self.central_widget.open_controller.clicked.connect(self.open_controller)

        QApplication.instance().aboutToQuit.connect(self.cleanup)

        self.setWindowTitle("Wavetable Synthesizer")
        self.setCentralWidget(self.central_widget)

        self.show()

    def cleanup(self):
        if self.sc_process != None:
            self.sc_process.kill()

    def parse_arguments(self, args = None):
        self.verbose = any(item in args[1:] for item in VERBOSITY_SETTING)

    def wait_for_boot(self):
        hung_counter = 0
        while True:
            output = self.sc_process.stdout.readline().decode("utf-8").strip()
            if self.verbose and output: 
                print(output)
            hung_counter = 0 if output else (hung_counter+1)
            if output == BREAK_COMMAND or hung_counter == 20:# break if hung
                break

    def open_controller(self):
        if self.controller_window == None:
            self.controller_window = Controller.Controller()
        elif not self.controller_window.isVisible():
            self.controller_window.setVisible(True)

    ### Chosen settings from user input
    def start_synth(self):
        for name, value in WIDGETS_PAIRS + WIDGET_LOAD_SEPARATELY:
            widget = getattr(self.central_widget, name)
            self.settings[value] = widget.currentText()
        helper.write_settings(self.settings, helper.SETTINGS_FILE)

        if self.sc_process == None:
            self.sc_process = subprocess.Popen(['/bin/bash', '-i', '-c', 'sclang supercollider/wavetable.scd'], stdout=subprocess.PIPE)
            self.wait_for_boot()

            self.central_widget.run_button.setText("Restart synth")
            self.central_widget.open_controller.setEnabled(True)
        elif self.controller_window != None:
            self.controller_window.send_trigger("/reboot")
            self.wait_for_boot()
        self.open_controller()

    def choose_project(self):
        self.chosen_project, filter_type = QFileDialog.getOpenFileName(self.central_widget, 'Open file', filter = "DIA project (*.dia)")

        if self.chosen_project:
            self.central_widget.project_name.setText(Path(self.chosen_project).stem)
            helper.change_enabled_settings(self.central_widget, exceptions = ["load_project", "open_controller"])

            imported_settings = helper.read_settings(self.chosen_project)
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

    def load_ports(self, device):
        helper.load_output_devices(device, self.central_widget.ports, AMT_CHANNELS)

    def load_midi_devices(self, default_device):
        midi_devices = self.central_widget.midi_devices
        midiout = rtmidi.MidiIn()

        for device in midiout.get_ports():
            midi_devices.addItem(device)
            if device == default_device:
                midi_devices.setCurrentIndex(midi_devices.count()-1) # device index

    def load_output_devices(self, default_device):
        output_devices = self.central_widget.output_devices
        output_devices.currentTextChanged.connect(self.load_ports)

        for device in sounddevice.query_devices():
            if device['max_output_channels'] >= AMT_CHANNELS:
                name = device['name']
                output_devices.addItem(name)
                if name == default_device:
                    output_devices.setCurrentIndex(output_devices.count()-1) # device index
                    self.load_ports(name)
                    
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
