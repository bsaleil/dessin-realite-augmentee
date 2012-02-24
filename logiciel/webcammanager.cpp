#include "webcammanager.hpp"

WebcamManager::WebcamManager()
{
}

// L'indice commence à 0
int WebcamManager::getNumberOfWebcams()
{
	CvCapture *capture;
	int nbCams = 0;
	
	// Essaye d'ouvrir toutes les webcams
	while(nbCams < 10)
	{
		capture = cvCaptureFromCAM(nbCams);
		if(!capture)
			return nbCams;
		else
		{
			cvReleaseCapture(&capture);	
			nbCams++;
		}
	}
	return nbCams;
}

IplImage* WebcamManager::getImageInit(int num)
{
	CvCapture *capture;

	// Ouverture de la webcam
	capture = cvCaptureFromCAM(num);
	if(!capture) 
	{
		cout << "Erreur à l'ouverture de la webcam" << endl;
		return NULL;
	}
		
	// Stocke l'image capturée
	IplImage *image = cvCloneImage(cvQueryFrame(capture));
	
	// Libère la webcam
	cvReleaseCapture(&capture);

	// Retourne l'image capturée
	return image;
}