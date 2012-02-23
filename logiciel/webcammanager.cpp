#include "webcammanager.hpp"

WebcamManager::WebcamManager()
{
}

int WebcamManager::getNumberOfWebcams()
{
	CvCapture *capture;
	int i=0;
	capture = cvCreateCameraCapture(i);
	
	while(capture != NULL)
	{
		capture = cvCreateCameraCapture(i);
		i++;
	}
	cvReleaseCapture(&capture);
	return i;
}
