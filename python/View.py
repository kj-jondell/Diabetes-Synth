import sys 
from PySide2.QtWidgets import (QApplication, QLabel, QPushButton,
                               QVBoxLayout, QWidget, QFileDialog,
                               QMainWindow)
from PySide2.QtCore import Slot, Qt, QFile, QIODevice
from PySide2.QtUiTools import QUiLoader

UI_FILE_NAME = "ui/form.ui" # Qt Designer ui file TODO fix path!

class View(QMainWindow):

    def __init__(self, parent = None):
        QMainWindow.__init__(self)

        self.load_view()
        self.central_widget.file_chooser.clicked.connect(self.choose_file)

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

    def get_settings(self):
        sample_rate = (int)(self.central_widget.sample_rate.currentText()) 
        buffer_size = (int)(self.central_widget.buffer_size.currentText())
        window_size = self.central_widget.window_size.value() 
        is_wavetable = self.central_widget.is_wavetable.isChecked() 
        write_file = self.central_widget.write_files.isChecked() 
        window_type = self.central_widget.window_type.currentText() 
        amt_output = self.central_widget.amt_files.value() 
        output_filename = ""

        if write_file:
            chosen_filename, filter_type = QFileDialog.getSaveFileName(self.central_widget, 'Save file', filter = "WAV files (*.wav)")
            aggregate_names = chosen_filename.rsplit(".")
            output_filename = "{}_{}.{}".format(aggregate_names[0], '{}', aggregate_names[1])

        return (self.chosen_filename, sample_rate, buffer_size,
                window_size, is_wavetable, write_file,
                window_type, amt_output, output_filename)

    def choose_file(self):
        self.chosen_filename, filter_type = QFileDialog.getOpenFileName(self.central_widget, 'Open file', filter = "XLS files (*.xls)")

        if self.chosen_filename != "":
            self.central_widget.filename.setText(self.chosen_filename)
            self.change_enabled_settings()

    # TODO group objects that are settings control instead of hardcoded hack !!!
    def change_enabled_settings(self, enable = True):
        for child in self.central_widget.children():
            if type(child) != QLabel and child.objectName() != "file_chooser":
                try: 
                    child.setEnabled(enable)
                except:
                    pass
