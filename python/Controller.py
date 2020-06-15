from PySide2.QtWidgets import QApplication, QProgressDialog 
from View import View  
from Model import Model
from PySide2.QtCore import Signal, Slot                            

class Controller():

    def __init__(self):
        self.view = View(self)
        self.view.central_widget.run_button.clicked.connect(self.convert_files)

    def convert_files(self):
        self.model = Model(*self.view.get_settings(), self)
        self.model.start()

        self.progress = QProgressDialog("Creating files", "Cancel", 0, 100, self.view)
        self.progress.canceled.connect(self.canceled)
        self.progress.setFixedWidth(self.progress.width() + 20) #Margin and fixed to avoid annoying jumping
        self.progress.show()

    #TODO delete written files (? or not? maybe a feature...)
    def canceled(self):
        self.model.requestInterruption()
        self.progress.setLabelText("Cancelling...")
        self.progress.setEnabled(False)
        self.progress.show()
        QApplication.instance().processEvents()
        while self.model.isRunning(): #wait until model is finished
            pass
        self.progress.hide()

    # This slot is called from the Model when it is working...
    @Slot(tuple) #TODO make into dict instead...
    def update_progressbar(self, args):
        self.progress.setValue(args[0])
        if args[1] != None:
            self.progress.setLabelText(args[1])

