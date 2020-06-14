import sys 
from PySide2.QtWidgets import (QApplication, QLabel, QPushButton,
                               QVBoxLayout, QWidget, QFileDialog,
                               QProgressDialog)
from PySide2.QtCore import Slot, Qt, QFile, QIODevice
from PySide2.QtUiTools import QUiLoader
from Model import Model

UI_FILE_NAME = "ui/controller.ui" # Qt Designer ui file TODO fix path!

class View(QApplication):

    def __init__(self):
        QApplication.__init__(self)

        self.load_view()
        self.window.file_chooser.clicked.connect(self.choose_file)
        self.window.run_button.clicked.connect(self.convert_files)

    def load_view(self):
        self.ui_file = QFile(UI_FILE_NAME)
        if not self.ui_file.open(QIODevice.ReadOnly):
            print("Cannot open {}: {}".format(UI_FILE_NAME, self.ui_file.errorString()))
            sys.exit(-1)

        self.loader =  QUiLoader()
        self.window = self.loader.load(self.ui_file)
        self.ui_file.close()

        if not self.window:
            print(self.loader.errorString())
            sys.exit(-1)

    def convert_files(self):
        sample_rate = (int)(self.window.sample_rate.currentText()) 
        buffer_size = (int)(self.window.buffer_size.currentText())
        window_size = self.window.window_size.value() 
        is_wavetable = self.window.is_wavetable.isChecked() 
        write_file = self.window.write_files.isChecked() 
        window_type = self.window.window_type.currentText() 
        amt_output = self.window.amt_files.value() 
        output_filename = ""

        if write_file:
            chosen_filename, filter_type = QFileDialog.getSaveFileName(self.window, 'Save file', filter = "WAV files (*.wav)")
            aggregate_names = chosen_filename.rsplit(".")
            output_filename = "{}_{}.{}".format(aggregate_names[0], '{}', aggregate_names[1])

        self.progress = QProgressDialog("Creating files", "Cancel", 0, 100, self.window)
        self.progress.show()
        self.update_progressbar()

        Model(self.chosen_filename, self, sample_rate, buffer_size,
                window_size, is_wavetable, write_file,
                window_type, amt_output, output_filename)

    def update_progressbar(self, value = 0, label = "Creating files"):
        self.progress.setValue(value)
        self.progress.setLabelText(label)
        self.processEvents()
        return self.progress.wasCanceled() #TODO change to threading with QRunnable...

    def choose_file(self):
        self.chosen_filename, filter_type = QFileDialog.getOpenFileName(self.window, 'Open file', filter = "XLS files (*.xls)")

        if self.chosen_filename != "":
            self.window.filename.setText(self.chosen_filename)
            self.change_enabled_settings()

    # TODO group objects that are settings control instead of hardcoded hack !!!
    def change_enabled_settings(self, enable = True):
        for child in self.window.centralwidget.children():
            if type(child) != QLabel and child.objectName() != "file_chooser":
                try: 
                    child.setEnabled(enable)
                except:
                    pass
        
    def run_view(self):
        self.window.show()
        sys.exit(self.exec_())
