#include "notebook_app.hpp"

//#include <InputWidget>
//#include <OutputWidget>

//#include <QLayout>
//#include <QDebug>

NotebookApp::NotebookApp(QWidget * parent): QWidget(parent){
	
	input = new InputWidget();
	input->setParent(parent);
    
	output = new OutputWidget();
	output->setParent(parent);
    
    QObject::connect(input, SIGNAL(textChanged(QString)),
      output, SLOT(showResult(QString)));
	
    auto layout = new QGridLayout();
	
	// layout->addWidget(widget, row, column)
	layout->addWidget(input, 0, 0);
	layout->addWidget(output, 1, 0);
	
	setLayout(layout);
}
