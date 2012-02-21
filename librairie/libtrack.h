#include <stdlib.h>
#include <stdio.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>

IplImage * Binarisation (IplImage * source, int h, int s, int v);
IplImage * Localisation (IplImage * binarySource, int x, int y);

int * NaifColorTrack (IplImage * source, int x, int y); 
