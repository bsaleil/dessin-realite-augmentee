#ifndef HPP_WEBCAMMANAGER
#define HPP_WEBCAMMANAGER

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;

class WebcamManager
{	
	public :
		WebcamManager();
		// Retourne le nombre de webcams disponibles
		int getNumberOfWebcams();
		// Retourne l'image donnée par la webcam
		IplImage* getImageInit(int);
};

#endif
