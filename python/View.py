import sys 
from PySide2.QtWidgets import (QApplication, QLabel, QPushButton,
                               QVBoxLayout, QWidget, QFileDialog)
from PySide2.QtCore import Slot, Qt

class View(QWidget):
    def __init__(self):
        QWidget.__init__(self)

        self.button = QPushButton("Choose file")
        self.label = QLabel("Chosen file: ")
        self.button.setMaximumSize(100, 30) #define some other way

        self.layout = QVBoxLayout()
        self.layout.addWidget(self.label)
        self.layout.addWidget(self.button)
        self.setLayout(self.layout)

        self.button.clicked.connect(self.chooseFile)

    def chooseFile(self):
        self.filename = QFileDialog.getOpenFileName(self, 'Open file')
        self.label.setText("Chosen file: {}".format(self.filename[0]))


app = QApplication(sys.argv)
view = View()
view.resize(800, 600)
view.show()

sys.exit(app.exec_())
