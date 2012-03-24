#ifndef HPP_WEBCAMS
#define HPP_WEBCAMS

/*
	-- Cette classe représente l'étape de choix de la webcam
*/

#include <iostream>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>
#include <QDialog>
#include <QTime>
#include <QMoveEvent>
#include <QMessageBox>
#include "webcammanager.hpp"
#include "viewetalonnage.hpp"
#include "../librairie/libtrack.hpp"

class Webcams : public QDialog
{	
	Q_OBJECT

	public :
		Webcams();	
	
	private :
		// Etat de l'étalonnage :
		int webcamChoisie;
		IplImage *imageCapturee;
		QPoint *pointChoisiA;
		QPoint *pointChoisiB;
		Cursor *curseur;
		
		QComboBox *choixWebcams;
		QVBoxLayout *layoutCentral;
		QPushButton *boutonPrecedent;
		QPushButton *boutonSuivant;
		
		int step;
		
		void clearLayout(QLayout *layout);
		QImage iplToQimage(IplImage* image);
	
		void loadClicksStep(int oldStep);
		void loadSettingsStep(int oldStep);
		void loadWebcamsStep(int oldStep);
	private slots :
		void slotPrevious();
		void slotNext();
		void slotNewPhoto();
};

#endif
