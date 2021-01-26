#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QList>
#include <QString>

#include <iostream>
#include <typeinfo>
#include <vector>

#define ORDER "order"
#define PROJECTNAME "projectname"
#define FILENAME "filename"
#define SAMPLERATE "samplerate"
#define NUMFRAMES "numframes"

using namespace std;

class CSVReader {

public:
  CSVReader(QString filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
      while (!file.atEnd()) {
        QList<QByteArray> line = file.readLine().split(',');
        parsedList[line[0]] = line[1].trimmed();
      }
    }
  };

  QMap<QString, QString> getParsed() { return parsedList; }

private:
  QMap<QString, QString> parsedList;
};
