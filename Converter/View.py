import sys
from PySide2.QtWidgets import (QApplication, QLabel, QPushButton,
                               QVBoxLayout, QWidget, QFileDialog,
                               QMainWindow)
from PySide2.QtCore import Slot, Qt, QFile, QIODevice
from PySide2 import QtXml
from PySide2.QtUiTools import QUiLoader
from pathlib import Path

UI_FILE_NAME = str(Path(__file__).parent / "ui/convert_gui.ui")

def change_enabled_settings(widgets, exceptions = ["file_chooser"], enable = True):
    for child in widgets.children():
        if type(child) != QLabel and child.objectName() not in exceptions:
            try: 
                child.setEnabled(enable)
            except:
                pass

class View(QMainWindow):

    def __init__(self, parent = None):
        QMainWindow.__init__(self)

        self.load_view()
        self.central_widget.file_chooser.clicked.connect(self.choose_file)

        self.setWindowTitle("Glucose Level Converter")
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
    
    # Returns user inputs if available
    def get_settings(self):
        settings = {}
        settings['chosen_filename'] = self.chosen_filename 
        settings['sample_rate'] = (int)(self.central_widget.sample_rate.currentText()) 
        settings['buffer_size'] = (int)(self.central_widget.buffer_size.currentText())
        settings['window_size'] = self.central_widget.window_size.value() 
        settings['is_wavetable'] = self.central_widget.is_wavetable.isChecked() 
        settings['window_type'] = self.central_widget.window_type.currentText() 
        settings['amt_output'] = self.central_widget.amt_files.value() 

        chosen_path, filter_type = QFileDialog.getSaveFileName(self.central_widget, 'Save project', filter = "DIA project")
        if chosen_path:
            chosen_path += "/samples"
            Path(chosen_path).mkdir(parents = True, exist_ok = True)
            settings['output_filename'] = "{}/sample_{}".format(chosen_path, '%1.wav')
            return settings 
        else:
            return None
    
    def choose_file(self):
        self.chosen_filename, filter_type = QFileDialog.getOpenFileName(self.central_widget, 'Open file', filter = "XLS files (*.xls)")

        if self.chosen_filename != "":
            self.central_widget.filename.setText(Path(self.chosen_filename).name)
            change_enabled_settings(self.central_widget)
