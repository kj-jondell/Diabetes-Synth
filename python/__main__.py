import sys
from python.wavetablesynth import Interface
from python.controller import Controller
from python.converter import Converter
from PySide2.QtWidgets import QApplication, QStyleFactory

DEFAULT = "synth"

app = QApplication([])
option_dict = {"controller" : Controller.Controller, "synth" : Interface.Interface, "converter" : Converter.Converter}

try:
    controller = option_dict[sys.argv[1]]()
except (IndexError, KeyError):
    controller = option_dict[DEFAULT]()

sys.exit(app.exec_())
