#ifndef HPP_CLIENT
#define HPP_CLIENT

#include <iostream>
#include <QMainWindow>
#include <QAction>
#include <QDebug>
#include <QMdiArea>
#include <QLabel>
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
		Ui::Client *ui;
		QActionGroup *actionGroup;
		QMdiArea *mdiArea;
		
		WebcamManager *camManager;
		WidgetWebcam *camWidget;
		
		void calibration();
		int calibrationCounter;
		QLabel *labelCounter;
		QTimer *timerCounter;
		int webcamActive; // Indice de la webcam utilisée
	
	private slots:
		void slotStart();
		void slotCounterChange();
		
};

#endif
