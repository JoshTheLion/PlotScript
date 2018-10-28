#include "input_widget.hpp"

#include <QWidget>
#include <QDebug>


InputWidget::InputWidget(QWidget * parent): QPlainTextEdit(parent){
	
    setOverwriteMode(true);
    setObjectName("input");
}

void InputWidget::keyPressEvent(QKeyEvent * keyEvent){
  
  qDebug() << keyEvent;
  
  auto k = keyEvent->key();
  auto m = keyEvent->modifiers();
  
  // Check for Shift+Enter
  if(m == Qt::ShiftModifier && (k == Qt::Key_Enter || k == Qt::Key_Return)){
    
    inputExp = QPlainTextEdit::toPlainText();
    qDebug() << "Signal: " << inputExp << "\n";

    // Send Expression to be evaluated
    emit textUpdated(inputExp);
  }
  else{
    // The inherited method handles all other events
    QPlainTextEdit::keyPressEvent(keyEvent);
  }
}


