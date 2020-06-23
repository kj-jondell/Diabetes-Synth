import sys 
from PySide2.QtWidgets import QMainWindow, QDial, QApplication
from PySide2.QtCore import Slot, Qt, QFile, QThread, QIODevice
from PySide2.QtUiTools import QUiLoader
from PySide2.QtNetwork import QUdpSocket, QHostAddress
from pythonosc import udp_client
from pythonosc.parsing import osc_types
from pathlib import Path
import asyncio

UI_FILE_NAME = str(Path(__file__).parent / "ui/synth-controller.ui") # Qt Designer ui file TODO fix path!
OSC_SEND_SETTINGS = ("127.0.0.1", 1121)
OSC_RECEIVE_PORT = 1122

class Controller(QMainWindow):

    def __init__(self, parent = None):
        QMainWindow.__init__(self)

        self.load_view()
        self.load_client()
        self.load_server()

        for dial in self.central_widget.findChildren(QDial):
                dial.valueChanged.connect(self.dial_change)

        self.setWindowTitle("Wavetable Controller")
        self.setCentralWidget(self.central_widget)

        self.show()

    def dial_change(self, value):
        self.client.send_message("/{}".format(self.sender().objectName()), int(value))

    def load_server(self):
        self.udpSocket = QUdpSocket(self)
        self.udpSocket.bind(OSC_RECEIVE_PORT)
        self.udpSocket.readyRead.connect(self.parse_udp)

    def parse_udp(self):
        while self.udpSocket.hasPendingDatagrams():
            datagram, host, port = self.udpSocket.readDatagram(self.udpSocket.pendingDatagramSize())
            address, index = osc_types.get_string(bytes(datagram), 0)
            data, index = osc_types.get_int(bytes(datagram), index+4) # TODO Why +4 needed?
            #TODO check address against list what to do

            widget = self.central_widget.findChild(QDial, address[1:])
            if widget:
                widget.setValue(data)

    def load_client(self):
        #UDP client
        self.client = udp_client.SimpleUDPClient(*OSC_SEND_SETTINGS)

    def load_view(self):
        self.ui_file = QFile(UI_FILE_NAME)
        if not self.ui_file.open(QIODevice.ReadOnly):
            print("Cannot open {}: {}".format(UI_FILE_NAME, self.ui_file.errorString()))
            sys.exit(-1)

        self.loader = QUiLoader()
        self.central_widget = self.loader.load(self.ui_file)
        self.ui_file.close()

        if not self.central_widget:
            print(self.loader.errorString())
            sys.exit(-1)
