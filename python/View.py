import sys 
from PySide2.QtWidgets import (QApplication, QLabel, QPushButton,
                               QVBoxLayout, QWidget, QFileDialog)
from PySide2.QtCore import Slot, Qt, QFile, QIODevice
from PySide2.QtUiTools import QUiLoader

UI_FILE_NAME = "ui/view.ui"

class View(QApplication):

    def __init__(self):
        QApplication.__init__(self)

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

        self.window.show()

        sys.exit(self.exec_())

View()

    # def __init__(self):
    #     QWidget.__init__(self)

    #     self.button = QPushButton("Choose file")
    #     self.label = QLabel("Chosen file: ")
    #     self.button.setMaximumSize(100, 30) #define some other way

    #     self.layout = QVBoxLayout()
    #     self.layout.addWidget(self.label)
    #     self.layout.addWidget(self.button)
    #     self.setLayout(self.layout)

    #     self.button.clicked.connect(self.chooseFile)

    # def chooseFile(self):
    #     self.filename = QFileDialog.getOpenFileName(self, 'Open file')
    #     self.label.setText("Chosen file: {}".format(self.filename[0]))
