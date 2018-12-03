#ifndef NOTEBOOK_APP_H
#define NOTEBOOK_APP_H

#include "input_widget.hpp"
#include "output_widget.hpp"
#include "interpreter.hpp"
#include "startup_config.hpp"
#include "message_queue.hpp"
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
    InputWidget * m_input_widget;
    OutputWidget * m_output_widget;
    
		// The thread-safe message queue channels for kernel I/O
		//InputQueue m_queue_in;
		//OutputQueue m_queue_out;
		
		// The interpreter kernel
		//Interpreter m_interp = Interpreter(&m_queue_in, &m_queue_out);
		Interpreter m_interp;

    // Mimick Plotscript to evaluate input
		//void threadEvalLoop();
		int startup(Interpreter & interp);
    bool evalExpression(std::string inExp);
    Settings errFormat(const std::string & message);

    // Assign result to QGraphicsItem equivalent
    Settings setGraphicsType(Expression exp);

  signals:

    void sendResult(Settings data);
};

#endif // NOTEBOOK_APP_H
