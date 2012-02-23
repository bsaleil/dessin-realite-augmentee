#ifndef HPP_CLIENT
#define HPP_CLIENT

#include <iostream>
#include <QMainWindow>
#include <QAction>
#include <QDebug>

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
		
		WebcamManager *camManager;
		WidgetWebcam *camWidget;
	
	private slots:
		void slotStart();
		
};

#endif
