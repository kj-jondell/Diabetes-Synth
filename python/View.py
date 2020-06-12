import sys 
from PySide2.QtWidgets import (QApplication, QLabel, QPushButton,
                               QVBoxLayout, QWidget, QFileDialog)
from PySide2.QtCore import Slot, Qt, QFile, QIODevice
from PySide2.QtUiTools import QUiLoader

UI_FILE_NAME = "ui/view.ui" # Qt Designer ui file

class View(QApplication):

    def __init__(self):
        QApplication.__init__(self)
        self.load_view()
        self.window.file_chooser.clicked.connect(self.choose_file)

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

    def choose_file(self):
        self.chosen_filename = QFileDialog.getOpenFileName(self.window, 'Open file', filter = "XLS files (*.xls)")
        if self.chosen_filename[0] != "":
            self.window.filename.setText(self.chosen_filename[0])
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

View().run_view()
