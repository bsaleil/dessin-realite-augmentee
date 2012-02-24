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
#include <QTimer>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "ui_client.h"
#include "webcammanager.hpp"
#include "widgetwebcam.hpp"

namespace Ui 
{
	class Client;
}

class Client : public QMainWindow
{
	Q_OBJECT	
	
	public :
		Client();
		
	private:
		Ui::Client *ui;			// Interface graphique
		QActionGroup *actionGroup;	// Groupe d'action pour les webcams
		QMdiArea *mdiArea;		// Zone multi fenetre
		QTimer *timerCounter;		// Timer pour compteur avant étalonnage
		
		int calibrationCounter;	// Compteur avant image pour étalonnage
		int webcamActive; 		// Indice de la webcam utilisée
		
		void calibration();		// Début de l'étalonnage
		
		WebcamManager *camManager;	// Classe gérant les webcams
		WidgetWebcam *camWidget;	// Widget d'affichage de la webcam
	
	private slots:
		void slotCounterChange();	// Slot appelé à chaque tick du compteur d'étalonnage
		void slotStart();		// Slot appelé lors du clic sur démarrer
		
		
};
#endif