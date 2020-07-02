#include "Controller.h"
#include <QObject>
#include <QtWidgets>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setApplicationName("Controller");

  SynthController controller;
  controller.show();

  QObject::connect(&app, &QApplication::aboutToQuit, &controller,
                   &SynthController::cleanupOnQuit);

  return app.exec();
}
