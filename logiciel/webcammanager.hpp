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
		
		IplImage* getImage();		// Retourne une image du flux
		IplImage* getImageInit(int);	// Retourne l'image donnée par la webcam
		int getNumberOfWebcams();	// Retourne le nombre de webcams disponibles
		// TODO : éventuellement un retour
		void runWebcam();		// Ouvre la webcam
		void stopWebcam();
		
	private :
		int webcam;
		CvCapture *capture;		// Capture de la webcam
};

#endif
