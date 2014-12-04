#include <iostream>
#include <fstream>
#include <stdlib.h>

#include <shotton/shotton.h>
#include <util/util.h>
#include <QVector>
#include <QString>
#include <QRgb>
#include <QDataStream>
#include <QFile>
#include <QString>
#include <QImage>
#include <QtGlobal>


int main(int argc,char * argv[])
{

  QString source = "sample.png";
  QString dumpfile = "VOC2010.ct";
  QImage img;
  img.load(source,"png");
  QVector<QRgb> colorTable = img.colorTable();
  QFile file(dumpfile);
  if (!file.open(QFile::WriteOnly))
              qFatal( "Failed to save means to '%s'", qPrintable( source ) );
  QDataStream s(&file);
  s << colorTable;
  file.close();

  
}