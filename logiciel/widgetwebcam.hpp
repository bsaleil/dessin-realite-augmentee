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

#include <opencv/cv.h>

#include "../librairie/libtrack.hpp"

class WidgetWebcam : public QLabel
{	
	Q_OBJECT
	
	public :
		WidgetWebcam();
		WidgetWebcam(QString); 			// QLabel avec texte à afficher
		
		void calibrate(IplImage*);		// Effectuer l'étalonnage sur l'image
		bool calibrationDone();		// Retourne vrai si l'étalonnage est effectué
		QImage iplToQimage(IplImage* image);	// Conversion d'une IplImage en QImage
		QPoint newImageFromWebcam(IplImage*);	// Reçoit une image depuis la webcam courante
		
	protected :
		void mousePressEvent(QMouseEvent*);	// Récupération du clic pour désigner objet à suivre
	private :
		bool calibrationIsDone;		// Indique si l'étalonnage est effectué
		bool readyToCalibrate; 		// Indique si prêt à recevoir le clic d'étalonnage
		IplImage* imageInit;			// Contient l'image pour l'initialisation
		Cursor cursor;				// Structure retournée lors du tracking par couleur
		QPixmap pixmapCourante;
};

#endif
