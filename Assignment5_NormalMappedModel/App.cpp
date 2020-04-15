#include "App.h"

#include "BasicWidget.h"

App::App(QWidget* parent, std::string objFilename) : QMainWindow(parent)
{
  buildGui(objFilename);
}

App::~App()
{}

void App::buildGui(std::string objFilename)
{
  // A simple menubar.
  QMenuBar* menu = menuBar();
  QMenu* file = menu->addMenu("File");
  QAction* exit = file->addAction("Quit", [this]() {close();});

  // Our basic widget.
  BasicWidget* widget = new BasicWidget(this, objFilename);
  setCentralWidget(widget);
}
