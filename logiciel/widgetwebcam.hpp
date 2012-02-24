#ifndef HPP_WIDGETWEBCAM
#define HPP_WIDGETWEBCAM

#include <iostream>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <opencv/cv.h>

class WidgetWebcam : public QLabel
{	
	Q_OBJECT
	
	public :
		WidgetWebcam();
		WidgetWebcam(QString); // QLabel avec texte
		
		// Pour initialiser la couleur à suivre
		void mousePressEvent(QMouseEvent*);
		void calibrate(IplImage*);

		// Conversion d'une IplImage en QImage
		QImage* iplToQimage(IplImage* image);
};

#endif
