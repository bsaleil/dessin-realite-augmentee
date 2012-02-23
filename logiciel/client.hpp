#ifndef HPP_CLIENT
#define HPP_CLIENT

#include <iostream>
#include <QMainWindow>

#include "ui_client.h"
#include "webcammanager.hpp"

class Client : public QMainWindow
{
	Q_OBJECT	
	
	public :
		Client();
	private:
		Ui::Client ui;
		
		WebcamManager *wm;
};

#endif
