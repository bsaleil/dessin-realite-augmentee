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
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QTimer>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "ui_client.h"
#include "webcammanager.hpp"
#include "widgetwebcam.hpp"
#include "localdrawingboard.hpp"
#include "networkdrawingboard.hpp"
#include "state.hpp"

namespace Ui 
{
	class Client;
}

class Client : public QMainWindow
{
	Q_OBJECT	
	
	public :
		Client(int, IplImage*, Cursor);		// Pour une utilisation locale
		Client(int, IplImage*, Cursor, QString, int);	// Pour une utilisation distante (hôte, port)

	private :
		void init(int, IplImage*, Cursor);			// Initialisation commune local-distant
	
		Ui::Client *ui;			// Interface graphique
		QScrollArea *scroll;		// Zone de défilement 
		
		DrawingBoard *drawingBoard;	// Widget de dessin
		WebcamManager *camManager;	// Classe gérant les webcams
		WidgetWebcam *camWidget;	// Widget d'affichage de la webcam
		
		IplImage *image;
		Cursor *curseur;
		
	protected :
		void keyPressEvent(QKeyEvent*);
		
	private slots :
		void tick();
		void exportDraw();		// Exporte le dessin courant 
		void flushScene();		// Réinitialise le tableau en le vidant
		void fullscreen();		// Affiche la scene en plein écran
		
};
#endif
