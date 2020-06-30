import sys, subprocess, rtmidi
from PySide2.QtWidgets import (QApplication, QLabel, QPushButton,
                               QVBoxLayout, QWidget, QFileDialog,
                               QMainWindow, QMenu, QMenuBar,
                               QComboBox)
from PySide2.QtCore import Slot, Qt, QFile, QIODevice
from PySide2.QtUiTools import QUiLoader
from python.controller import Controller
from python.converter import Converter
import sounddevice 
import csv
from pathlib import Path
import python.helper.helper as helper
from python.helper.helper import Settings

VERBOSITY_SETTING =  ['-v', '--verbose']

BREAK_COMMAND = "startup finished"
UI_FILE_NAME = str(Path(__file__).parent / "ui/wavetable_gui.ui") # Qt Designer ui file TODO fix path!
WIDGETS_PAIRS = [('buffer_size', 'numframes'), ('sample_rate', 'samplerate'), ('memory_size', 'memsize')]
WIDGET_LOAD_SEPARATELY = [('output_devices', 'device'), ('midi_devices', 'mididevice'), ('ports', 'port'), ('channels', 'midichannel'), ('project_name', 'projectname')]

class Interface(QMainWindow):

    def __init__(self, args = None):
        QMainWindow.__init__(self)

        self.parse_arguments(args)

        self.sc_process = None
        self.controller_window = None
        self.converter_window = None

        self.load_view()

        self.settings = Settings()
        self.loaded_settings = True
        if not self.settings.read_settings(): #Import settings
            helper.change_enabled_settings(self.central_widget, exceptions = ["load_project", "open_controller", "open_converter"], enable = False) #if no default project
            self.loaded_settings = False

        self.load_output_devices(self.settings.device)
        self.load_midi_devices(self.settings.mididevice)

        self.load_settings()

        self.central_widget.load_project.clicked.connect(self.choose_project)
        self.central_widget.run_button.clicked.connect(self.start_synth)
        self.central_widget.open_controller.clicked.connect(self.open_controller)
        self.central_widget.open_converter.clicked.connect(self.open_converter)

        QApplication.instance().aboutToQuit.connect(self.cleanup)

        self.setWindowTitle("Wavetable Synthesizer")
        self.setCentralWidget(self.central_widget)

        self.show()

    # This slot is called from the Controller when midichannel has changed
    @Slot(int) 
    def midichannel_change(self, index):
        self.central_widget.channels.setCurrentIndex(index)

    # This slot is called from the Controller when port has changed
    @Slot(int) 
    def port_change(self, index):
        self.central_widget.ports.setCurrentIndex(index)

    def cleanup(self):
        if self.loaded_settings:
            self.update_settings()

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
            if output == BREAK_COMMAND:
                break
            elif hung_counter == 20:# break if hung TODO preferably the program should try to restart supercollider (but currently breaks..)
                sys.exit(-1)

    def open_converter(self):
        if self.converter_window == None:
            self.converter_window = Converter.Converter()
        elif not self.converter_window.view.isVisible():
            self.converter_window.view.setVisible(True)

    def open_controller(self):
        if self.controller_window == None:
            self.controller_window = Controller.Controller(parent=self)
        elif not self.controller_window.isVisible():
            self.controller_window.setVisible(True)
        self.controller_window.load_output_settings_from_file()

    def update_settings(self):
        # LOAD comboboxes
        for name, key in WIDGETS_PAIRS + WIDGET_LOAD_SEPARATELY:
            widget = getattr(self.central_widget, name)
            setattr(self.settings, key, widget.currentText() if type(widget) == QComboBox else widget.text())
        self.settings.availableports = sounddevice.query_devices(self.settings.device)['max_output_channels'] #TODO should be done in supercollider ( if possible? )
        self.settings.write_settings()

    ### Chosen settings from user input
    def start_synth(self):
        self.update_settings()

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
            helper.change_enabled_settings(self.central_widget, exceptions = ["load_project", "open_controller"])

            self.settings.merge_settings(path=self.chosen_project)
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
        helper.load_output_devices(device, self.central_widget.ports, helper.AMT_CHANNELS)

    def load_midi_devices(self, default_device):
        midi_devices = self.central_widget.midi_devices
        midiout = rtmidi.MidiIn()
        helper.load_midi_ports(self.central_widget.channels)

        for device in midiout.get_ports():
            midi_devices.addItem(device)
            if device == default_device:
                midi_devices.setCurrentIndex(midi_devices.count()-1) # device index

        self.central_widget.channels.setCurrentIndex(self.settings.get_midi_device_index())

    def load_output_devices(self, default_device):
        output_devices = self.central_widget.output_devices
        output_devices.currentTextChanged.connect(self.load_ports)

        for device in sounddevice.query_devices():
            if device['max_output_channels'] >= helper.AMT_CHANNELS:
                name = device['name']
                output_devices.addItem(name)
                if name == default_device:
                    output_devices.setCurrentIndex(output_devices.count()-1) # device index
                    self.load_ports(name)

        self.central_widget.ports.setCurrentIndex(self.settings.get_ports_index()) 
                    
    def load_settings(self):
        projectname = self.settings.projectname
        if projectname:
            self.central_widget.project_name.setText(projectname)
            for pair in WIDGETS_PAIRS:
                self.load_setting(*pair)
        else:
            self.central_widget.project_name.setText("Project not found")

        self.loaded_settings = bool(projectname)

    def load_setting(self, widget_name, key):
        widget = getattr(self.central_widget, widget_name)

        default_key = getattr(self.settings, key)
        for index in range(widget.count()):
            if widget.itemText(index) == default_key:
                widget.setCurrentIndex(index)
                break
