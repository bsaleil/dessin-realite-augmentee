#ifndef HPP_WIDGETWEBCAM
#define HPP_WIDGETWEBCAM

#include <iostream>
#include <QWidget>

class WidgetWebcam : public QWidget
{	
	Q_OBJECT
	
	public :
		WidgetWebcam();
		
		// Pour initialiser la couleur à suivre
		void mousePressEvent(QMouseEvent*);
};

#endif
