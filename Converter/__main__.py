import sys
from Converter import Converter
from PySide2.QtWidgets import QApplication, QStyleFactory

app = QApplication([])
controller = Converter()

sys.exit(app.exec_())
