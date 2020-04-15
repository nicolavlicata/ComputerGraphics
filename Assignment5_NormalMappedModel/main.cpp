/**
 * Support code written by Erik W. Anderson
 */

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>
// Lab application
#include "App.h"
#include <iostream>

static bool enableGLDebug = true;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage " << argv[0] << " [.obj file path]" << std::endl;
    return 1;
  }
  std::cout << "main " << argv[1] << std::endl;
  QApplication a(argc, argv);
  QString appDir = a.applicationDirPath();
  QDir::setCurrent(appDir);

  QSurfaceFormat fmt;
  fmt.setDepthBufferSize(24);
  fmt.setStencilBufferSize(8);
  fmt.setVersion(3,3);
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  if(enableGLDebug) {
    fmt.setOption(QSurfaceFormat::DebugContext);
  }
  QSurfaceFormat::setDefaultFormat(fmt);
 
  std::string objFilename(argv[1]);
  App app(0, objFilename);
  app.show();
  return QApplication::exec();
}
