#include "Controller.h"
#include <QtWidgets>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setApplicationName("Controller");

  SynthController controller;
  controller.show();

  return app.exec();
}
