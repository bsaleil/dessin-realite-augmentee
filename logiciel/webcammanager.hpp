#ifndef HPP_WEBCAMMANAGER
#define HPP_WEBCAMMANAGER

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class WebcamManager
{	
	public :
		WebcamManager();
		// Get number of webcams
		int getNumberOfWebcams();
};

#endif
