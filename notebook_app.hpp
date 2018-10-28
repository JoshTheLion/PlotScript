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
//#include "startup_config.hpp" // Access to startup.pls

class NotebookApp: public QWidget
{
Q_OBJECT

public:
	NotebookApp(QWidget * parent = nullptr);

private slots:
	void getUserText(QString inExp);

signals:
    void resultEvaluated(QString outExp);

private:
	InputWidget * input;
	OutputWidget * output;
    Interpreter m_interp;
    //QString m_exp;

    // Mimick Plotscript
    QString evalExpression(std::string inExp);
    
};
#endif // NOTEBOOK_APP_H
