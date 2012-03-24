#ifndef HPP_WIDGETWEBCAM
#define HPP_WIDGETWEBCAM

/*
	-- Cette classe se charge de l'appel à la librairie de trace et de l'affichage de l'image webcam
	-- Elle permet de faire le lien entre l'interface, et la librairie de tracage.
	-- Dépendante de Qt et openCV et de la librairie de track
*/

#include <iostream>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <opencv/cv.h>

#include "../librairie/libtrack.hpp"

class WidgetWebcam : public QLabel
{	
	Q_OBJECT
	
	public :
		WidgetWebcam();
		QPoint getNewPosition(IplImage*,Cursor*);	// Affiche l'image et retourne la nouvelle position de l'objet
		
	private :
		
		QImage iplToQimage(IplImage* image);	// Conversion d'une IplImage en QImage
};

#endif
