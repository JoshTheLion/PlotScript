#ifndef NOTEBOOK_APP_H
#define NOTEBOOK_APP_H

#include <QWidget>
#include <QLayout>
#include <QDebug>
#include <QString>
#include <string>

#include "input_widget.hpp"
#include "output_widget.hpp"
#include "interpreter.hpp"  // Access to PlotScript
#include "startup_config.hpp" // Access to startup.pls

class NotebookApp: public QWidget
{
Q_OBJECT

public:
	NotebookApp(QWidget * parent = nullptr);

private slots:
	void getUserText(QString inExp);

signals:
    //void resultEvaluated(QString outExp);
    void graphicsResult(QGraphicsItem * item);

private:
	InputWidget * input;
	OutputWidget * output;
    Interpreter m_interp;
    //QString m_strResult;
    //Expression m_expResult;

    // Mimick Plotscript
    int startup(Interpreter & interp);
    void evalExpression(std::string inExp);
    QGraphicsTextItem * errFormat(std::string message);
    
    // Assign result to QGraphicsItem equivalent
    void setGraphicsType(Expression exp);
};
#endif // NOTEBOOK_APP_H
