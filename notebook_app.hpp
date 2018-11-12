#ifndef NOTEBOOK_APP_H
#define NOTEBOOK_APP_H

#include "input_widget.hpp"
#include "output_widget.hpp"
#include "interpreter.hpp"
#include "startup_config.hpp"
#include "settings.h"

#include <QWidget>
#include <QLayout>
#include <QDebug>
#include <QVector>
#include <QPoint>
#include <QString>
#include <string>

class NotebookApp : public QWidget {
  Q_OBJECT

  public:
    
    NotebookApp(QWidget * parent = nullptr);

  public slots:
    
    void getUserText(QString inExp);

  private:
    
    // Composite objects
    InputWidget * m_input;
    OutputWidget * m_output;
    Interpreter m_interp;

    // Mimick Plotscript to evaluate input
    int startup(Interpreter & interp);
    bool evalExpression(std::string inExp);
    Settings errFormat(const std::string & message);

    // Assign result to QGraphicsItem equivalent
    Settings setGraphicsType(Expression exp);

  signals:

    void sendResult(Settings data);
};

#endif // NOTEBOOK_APP_H
