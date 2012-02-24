#ifndef HPP_WIDGETWEBCAM
#define HPP_WIDGETWEBCAM

/*
	-- Cette classe se charge de l'appel à la librairie de trace et de l'affichage de l'image webcam
	-- Elle permet de faire le lien entre l'interface, et la librairie de tracage.
	-- Dépendante de Qt et openCV
*/

#include <iostream>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>

#include <opencv/cv.h>

class WidgetWebcam : public QLabel
{	
	Q_OBJECT
	
	public :
		WidgetWebcam();
		WidgetWebcam(QString); 			// QLabel avec texte à afficher
		
		void mousePressEvent(QMouseEvent*);	// Récupération du clic pour désigner objet à suivre
		void calibrate(IplImage*);		// Effectuer l'étalonnage sur l'image

		QImage* iplToQimage(IplImage* image);	// Conversion d'une IplImage en QImage
	
	private :
		bool readyToCalibrate; 		// Indique si prêt à recevoir le clic d'étalonnage
};

#endif