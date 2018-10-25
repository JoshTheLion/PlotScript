#include <QApplication>
#include <QWidget>

#include "notebook_app.hpp"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  NotebookApp widget;

  widget.show();
  
  return app.exec();
}

