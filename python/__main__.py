### Interpreter of blood glucose data from Abbot FreeStyle Libre. Spline interpolation and ouptut of 1024 sample wav-file.
### TODO: 1. Cleanup code
### 2. Threading (with QRunnable)!!! Better handling of GUI thread and Model. (Semaphore?) Signals/Slots...
### 3. View inheriting from QWindow instead of QApplication
### 4. Standalone executable (PyInstaller)
### 5. Implement centroids!
### 
### Make both runnable as standalone AND integrated app with SuperCollider synth

import sys
from PySide2.QtWidgets import QApplication, QProgressDialog 
from View import View  
from Model import Model
from PySide2.QtCore import Signal, Slot                            

class Controller():

    def __init__(self):
        self.app = QApplication(sys.argv) 
        self.view = View(self)

        self.view.central_widget.run_button.clicked.connect(self.convert_files)

        sys.exit(self.app.exec_())

    def convert_files(self):
        self.model = Model(*self.view.get_settings(), self)
        self.model.start()

        self.progress = QProgressDialog("Creating files", "Cancel", 0, 100, self.view)
        self.progress.canceled.connect(self.model.requestInterruption)
        self.progress.show()

    @Slot(tuple) #TODO make into dict instead...
    def update_progressbar(self, args):
        self.progress.setValue(args[0])
        self.progress.setLabelText(args[1])
        self.app.processEvents()

if __name__ == "__main__":
    Controller()
