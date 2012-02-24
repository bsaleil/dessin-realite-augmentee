#ifndef HPP_WEBCAMMANAGER
#define HPP_WEBCAMMANAGER

/*
	-- Cette classe se charge de gérer les webcams (ouverture, lecture, etc...)
	-- Elle fait le lien entre l'interface graphique client et le matériel (webcams)
	-- Dépendante d'openCV
*/

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;

class WebcamManager
{	
	public :
		WebcamManager();
		int getNumberOfWebcams();	// Retourne le nombre de webcams disponibles
		IplImage* getImageInit(int);	// Retourne l'image donnée par la webcam
};

#endif
