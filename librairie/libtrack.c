#include "libtrack.h"

IplImage * Binarisation(IplImage * source, int x, int y)
{
	IplImage *hsv;
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV);
	CvScalar pixel;
	pixel = cvGet2D(hsv,x,y); // on recupère le pixel
	
	int h = (int)pixel.val[0];
	int s = (int)pixel.val[1];
	int v = (int)pixel.val[2];
	int tolerance = 10;
	IplImage *mask = NULL;
	mask = cvCreateImage(cvGetSize(source), source->depth, 1);
	cvInRangeS(hsv, cvScalar(h - tolerance -1, s - tolerance, 0), cvScalar(h + tolerance -1, s + tolerance, 255), mask);
	return mask;
}

IplImage * Localisation(IplImage * binarySource, int x, int y)
{

}

int * NaifColorTrack(IplImage * source, int x, int y)
{

}

int main(int argc, char* argv[])
{
	IplImage* source = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
	cvNamedWindows("test", CV_WINDOW_AUTOSIZE);
	cvShowImage("test", Binarisation(source, 25, 25));
	
}
