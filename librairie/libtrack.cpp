#include "libtrack.hpp"
using namespace std;

IplImage * Binarisation(IplImage * source, int x, int y)
{
	IplImage *hsv;
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV);
	CvScalar pixel = cvGet2D(hsv,x,y); // on recupère le pixel
	
	int h = (int)pixel.val[0];
	int s = (int)pixel.val[1];
	int v = (int)pixel.val[2];
	int tolerance = 20;
	IplImage *mask = NULL;
	mask = cvCreateImage(cvGetSize(source), source->depth, 1);
	cvInRangeS(hsv, cvScalar(h - tolerance -1, s - tolerance, 0,0), cvScalar(h + tolerance -1, s + tolerance, 255,0), mask);
	
	//Il convient ensuite d'appliquer une ouverture (dilatation puis érosion) à notre image, 
	//afin d'éliminer les zones non pertinentes tout en améliorant la perception de l'objet
	IplConvKernel *structurant;
/*	structurants possibles : 
	CV_SHAPE_RECT
    CV_SHAPE_CROSS
    CV_SHAPE_ELLIPSE
    CV_SHAPE_CUSTOM ==> int* à passer dans le paramètre value (dernier param) 
*/    
	structurant = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_ELLIPSE, NULL);
	cvDilate(mask, mask, structurant, 1);
	cvErode(mask, mask, structurant, 1);
	
	return mask;
}

/**
 * Retourne le centre de gravité des pixels blancs d'une image binarisée sous la forme d'une struc Coord (int x, int y)
 */
Coord getGravityCenter (IplImage* imgBin)
{
	assert (imgBin->nChannels == 1 );
	
	int sommeX = 0;
	int sommeY = 0;
	int nbPixels = 0;
	for(int x = 0; x < imgBin->width; x++)
		for(int y = 0; y < imgBin->height; y++)
			if(((uchar *)(imgBin->imageData + y*imgBin->widthStep))[x] == 255)
			{
				sommeX += x;
				sommeY += y;
				nbPixels++;
			}
			
	Coord bary;
	bary.x = (int)(sommeX / nbPixels);
	bary.y = (int)(sommeY / nbPixels);
	
	return bary;
}

/*
 *	getNewCoord(image binaire,ancienne position) retourne nouvelle position.
 * \TODO
 */
Coord getNewCoord(const IplImage* binaryImg, Coord oldCoord)
{
	Coord c = oldCoord;
	
	//Vérif que l'image donnée soit binarisée
	assert (binaryImg->nChannels != 1 );
	
	
	return c;
}


int * NaifColorTrack(IplImage * source, int x, int y)
{

}

int main(int argc, char* argv[])
{
	IplImage* source = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
	//cvNamedWindow("test", CV_WINDOW_AUTOSIZE);
	cvShowImage("source", source);
	cvWaitKey(0);
	cvShowImage("binarisation", Binarisation(source, 164, 527));
	cvWaitKey (0);
  	/* Destruction de la fenêtre */
	cvDestroyAllWindows ();
	/* Libération de la mémoire */
	cvReleaseImage (&source);
	
}
