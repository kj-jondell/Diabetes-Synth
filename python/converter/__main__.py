### Interpreter of blood glucose data from Abbot FreeStyle Libre. Spline interpolation and ouptut of wav-file.
### TODO: 1. Cleanup code
### 2. Standalone executable (PyInstaller)
### 3. Implement centroids!
### 4. Clean up files when cancelled??
### 5. Rename View and Controller to better fit MVC-architechture
### 6. Instead of option "write file" have a option for centroids!
### 
### Make both runnable as standalone AND integrated app with SuperCollider synth

import sys
from python.converter.Controller import Controller 
from PySide2.QtWidgets import QApplication

if __name__ == "__main__":
    app = QApplication(sys.argv)
    controller = Controller()
    sys.exit(app.exec_())
