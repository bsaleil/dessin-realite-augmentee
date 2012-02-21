#include <stdlib.h>
#include <stdio.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>

IplImage * Binarisation(IplImage * source, int x, int y);
IplImage * Localisation(IplImage * binarySource, int x, int y);

int * NaifColorTrack(IplImage * source, int x, int y); 
