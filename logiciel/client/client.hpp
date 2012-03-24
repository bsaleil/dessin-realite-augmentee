#ifndef HPP_CLIENT
#define HPP_CLIENT

/*
	-- Cette classe représente l'interface graphique du client.
	-- Elle permet également de faire le lien entre la classe WebcamManager, l'interface, et la classe WidgetWebcam.
	-- Dépendante de Qt et openCV
*/

#include <iostream>
#include <QAction>
#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTimer>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "ui_client.h"
#include "webcammanager.hpp"
#include "widgetwebcam.hpp"
#include "localdrawingboard.hpp"
#include "networkdrawingboard.hpp"

namespace Ui 
{
	class Client;
}

class Client : public QMainWindow
{
	Q_OBJECT	
	
	public :
		Client(int, IplImage*, Cursor);		// Pour une utilisation locale
		Client(int, IplImage*, Cursor, QString, int);	// Pour une utilisation distante (hôte,port)

	private :
		Ui::Client *ui;			// Interface graphique
		QMdiArea *mdiArea;		// Zone multi fenetre
		
		DrawingBoard *drawingBoard;	// Widget de dessin
		WebcamManager *camManager;	// Classe gérant les webcams
		WidgetWebcam *camWidget;	// Widget d'affichage de la webcam
		
		IplImage *image;
		Cursor *curseur;
		
	private slots :
		void tick();
		
};
#endif
