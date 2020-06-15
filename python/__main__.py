### Interpreter of blood glucose data from Abbot FreeStyle Libre. Spline interpolation and ouptut of wav-file.
### TODO: 1. Cleanup code
### 2. Threading (with QRunnable)!!! Better handling of GUI thread and Model. (Semaphore?) Signals/Slots... (DONE)
### 3. View inheriting from QWindow instead of QApplication (DONE)
### 4. Standalone executable (PyInstaller)
### 5. Implement centroids!
### 
### Make both runnable as standalone AND integrated app with SuperCollider synth

import sys
from Controller import Controller 
from PySide2.QtWidgets import QApplication

if __name__ == "__main__":
    app = QApplication(sys.argv)
    controller = Controller()
    sys.exit(app.exec_())
