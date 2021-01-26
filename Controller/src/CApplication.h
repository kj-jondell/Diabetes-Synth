#include <QApplication>
#include <QFileOpenEvent>
#include <QString>
#include <QtDebug>

class CApplication : public QApplication {
  Q_OBJECT

public:
  CApplication(int &argc, char **argv) : QApplication(argc, argv) {}

  bool event(QEvent *event) override {

    if (event->type() == QEvent::FileOpen) {
      QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
      emit fileOpen(openEvent->file());
    }

    return QApplication::event(event);
  }

signals:
  void fileOpen(QString);
};
