### TODO: save default settings
import sys
from Interface import Interface
from PySide2.QtWidgets import QApplication

if __name__ == "__main__":
    app = QApplication(sys.argv)
    controller = Interface()
    sys.exit(app.exec_())
