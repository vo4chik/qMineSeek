#ifndef RBUTTON_H
#define RBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QApplication>

class RButton : public QPushButton
{
	Q_OBJECT
protected:
	void mouseReleaseEvent(QMouseEvent* e) {
		//if (e->button() == Qt::RightButton) emit rightClicked();
		//if (e->button() == Qt::MiddleButton) emit middleClicked();
		if (e->button() != Qt::LeftButton)
			emit nonLclicked(*e, QApplication::keyboardModifiers());
		QPushButton::mouseReleaseEvent(e);
	}
public:
	RButton(QWidget* parent = nullptr);
signals:
	//Q_DECL_DEPRECATED
	//void rightClicked();
	//Q_DECL_DEPRECATED
	//void middleClicked();
	
	void nonLclicked(QMouseEvent me, Qt::KeyboardModifiers k);
};

#endif // RBUTTON_H
