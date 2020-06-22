### TODO: 1. save default settings
### 2. Allow loading different settings 

import sys
from python.wavetablesynth.Interface import Interface
from PySide2.QtWidgets import QApplication

if __name__ == "__main__":
    app = QApplication(sys.argv)
    controller = Interface()
    sys.exit(app.exec_())
