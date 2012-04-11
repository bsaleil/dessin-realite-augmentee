#include "webcammanager.hpp"

WebcamManager::WebcamManager()
{
}

IplImage* WebcamManager::getImage()
{
	// Capture d'une image de la webcam
	return cvQueryFrame(capture);
}

IplImage* WebcamManager::getImageInit(int num)
{
	webcam = num;
	CvCapture *captureInit;

	// Ouverture de la webcam
	captureInit = cvCaptureFromCAM(num);
	if(!captureInit) 
	{
		cout << "Erreur à l'ouverture de la webcam" << endl;
		return NULL;
	}
	
	// Pour permettre un calibrage de la lumière
	for (int i=0; i<10;i++)
		cvQueryFrame(captureInit);
	
	// Stocke l'image capturée
	IplImage *image = cvCloneImage(cvQueryFrame(captureInit));
	
	// Libère la webcam
	cvReleaseCapture(&captureInit);

	// Retourne l'image capturée
	return image;
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

int WebcamManager::runWebcam()
{
	capture = cvCaptureFromCAM(webcam);
	
	if(capture == 0)
	{
		cout << "Échec à l'ouverture de la caméra";
		return -1;
	}
	
	return 0;
}

void WebcamManager::setWebcam(int w)
{
	webcam = w;
}

void WebcamManager::stopWebcam()
{
	cvReleaseCapture(&capture);
}

