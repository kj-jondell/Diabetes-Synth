from PySide2.QtWidgets import QApplication, QProgressDialog 
from View import View  
from Model import Model
from PySide2.QtCore import Signal, Slot, Qt 
import numpy, sys
import csv
from pathlib import Path

def write_settings(settings_dict, path):
    with open(path, 'w') as csvfile:
        writer = csv.writer(csvfile)
        for value in settings_dict:
            writer.writerow([value, " "+ settings_dict[value]])

def list_to_string(input_list = []):
    return str(input_list)[1:-1].replace(',', '')

class Converter():

    def __init__(self, args = None):
        self.view = View(self)
        self.view.central_widget.run_button.clicked.connect(self.convert_files)

    def convert_files(self):
        self.centroids = None # IF user quits conversion before any waveforms are generated
        self.settings = self.view.get_settings()
        if self.settings == None: # IF filedialog is cancelled then stop!
            return

        self.model = Model(**self.settings, parent=self)
        self.model.finished.connect(self.update_settings)
        self.model.start()

        self.progress = QProgressDialog("Creating files", "Cancel", 0, 100, self.view, Qt.WindowStaysOnTopHint)
        self.progress.canceled.connect(self.canceled)
        self.progress.setFixedWidth(self.progress.width() + 20) #Margin and fixed to avoid annoying jumping
        self.progress.show()

    #TODO delete written files (? or not? maybe a feature...)
    def canceled(self):
        self.model.requestInterruption()
        self.progress.setLabelText("Cancelling...")
        self.progress.setEnabled(False)
        self.progress.show()
        self.progress.activateWindow()
        QApplication.instance().processEvents()
        while self.model.isRunning(): #wait until model is finished
            pass
        self.progress.hide()

    # Set icon of project (if cocoa package is available) ONLY WORKS ON MAC OS
    def set_icon(self, file_name):
        try:
            from Cocoa import NSImage, NSWorkspace
            path = (sys._MEIPASS+"/icon.icns/icon.icns") if hasattr(sys, "_MEIPASS") else "icon.icns"
            img = NSImage.alloc().initWithContentsOfFile_(str(path))
            NSWorkspace.sharedWorkspace().setIcon_forFile_options_(img, str(file_name), 0)
        except ModuleNotFoundError:
            pass #silent fail writing icon 

    # This slot is called from the Model when it is working...
    @Slot(tuple) #TODO make into dict instead...
    def update_progressbar(self, args):
        self.progress.setValue(args[0])
        if args[1] != None:
            self.progress.setLabelText(args[1])

    @Slot(list)
    def update_centroids(self, centroids):
        self.centroids = centroids

    def update_settings(self):
        if self.settings['is_wavetable'] and self.centroids != None:
            project_path = Path(self.settings['output_filename']).parents[1]
            project_setting_name = (project_path/project_path.stem).with_suffix(".dia")

            settings_dict = {}
            settings_dict['numframes'] = str(self.settings['buffer_size'])
            settings_dict['filename'] = self.settings['output_filename']
            settings_dict['order'] = list_to_string(list(numpy.argsort(self.centroids)+1))
            settings_dict['samplerate'] = str(self.settings['sample_rate'])
            settings_dict['projectname'] = project_setting_name.stem

            write_settings(settings_dict, project_setting_name)

            self.set_icon(project_path)
            self.set_icon(project_setting_name)
