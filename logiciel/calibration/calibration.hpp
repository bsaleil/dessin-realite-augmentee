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
#include <QRadioButton>
#include <QSpinBox>
#include <QLineEdit>
#include "webcammanager.hpp"
#include "viewcalibration.hpp"
#include "../librairie/libtrack.hpp"
#include "client.hpp"

class Calibration : public QWidget
{	
	Q_OBJECT

	public :
		Calibration();	
	
	private :
		// Etat de l'étalonnage :
		int webcamChoisie;
		QPoint *pointChoisiA;
		QPoint *pointChoisiB;
		
		// Utilisé pour le track
		IplImage *imageCapturee;
		Cursor *curseur;
		
		QComboBox *choixWebcams;
		QVBoxLayout *layoutCentral;
		QPushButton *boutonPrecedent;
		QPushButton *boutonSuivant;
		
		QLabel *labelSlider;
		QLabel *imageSettings;
		QRadioButton *boutonDistant;
		
		QSpinBox *spinPort;
		QLineEdit *editAdresse;
		
		int step;
		
		void clearLayout(QLayout *layout);
		QImage iplToQimage(IplImage* image);
	
		void loadClicksStep(int oldStep);
		void loadSettingsStep(int oldStep);
		void loadWebcamsStep(int oldStep);
		void loadNetworkStep(int oldStep);
		void launchClient();
	private slots :
		void slotPrevious();
		void slotNext();
		void slotNewPhoto();
		void slotSliderSettings(int value);
};

#endif
