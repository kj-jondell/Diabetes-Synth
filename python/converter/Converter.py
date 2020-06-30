from PySide2.QtWidgets import QApplication, QProgressDialog 
from python.converter.View import View  
from python.converter.Model import Model
from PySide2.QtCore import Signal, Slot, Qt 
import numpy 
import csv
from pathlib import Path
from python.helper.helper import Settings
import python.helper.helper as helper

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
            project_path = (project_path/project_path.stem).with_suffix(".dia")

            output_settings = Settings()
            output_settings.numframes = self.settings['buffer_size']
            output_settings.filename = self.settings['output_filename'].format('%')
            output_settings.order = helper.list_to_string(list(numpy.argsort(self.centroids)+1))
            output_settings.samplerate = self.settings['sample_rate']
            output_settings.projectname = project_path.stem

            output_settings.write_settings(project_path)
