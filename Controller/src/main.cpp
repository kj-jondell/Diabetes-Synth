#include "CApplication.h"
#include "Controller.h"
#include <QFileOpenEvent>
#include <QObject>
#include <QtWidgets>

int main(int argc, char *argv[]) {
  CApplication app(argc, argv);
  app.setApplicationName("Controller");

  SynthController controller;
  controller.show();

  CApplication::connect(&app, &CApplication::aboutToQuit, &controller,
                        &SynthController::cleanupOnQuit);

  CApplication::connect(&app, &CApplication::fileOpen, &controller,
                        &SynthController::fileOpen);

  return app.exec();
}
