#include "libtrack.hpp"
using namespace std;

// Retourne l'image binarisée de 'source' en fonction des informations contenues dans le 'oldPixel' (coord et coul)
IplImage * binarisation(IplImage * source, Pixel *oldPix)
{
	IplImage *hsv;
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV); // on travaille sur l'image en hsv => permet d'ignorer la luminosité
	CvScalar pixel =  oldPix->color; // permet de récupérer la couleur à traquer.
	
	int h = (int)pixel.val[0];
	int s = (int)pixel.val[1];
	int v = (int)pixel.val[2];
	int tolerance = 20; // à passer en paramètre d'ajustement??
	
	IplImage *mask = NULL;
	mask = cvCreateImage(cvGetSize(source), source->depth, 1);
	cvInRangeS(hsv, cvScalar(h - tolerance -1, s - tolerance, 0,0), cvScalar(h + tolerance -1, s + tolerance, 255,0), mask);
	
	//Il convient ensuite d'appliquer une ouverture (dilatation puis érosion) à notre image, 
	//afin d'éliminer les zones non pertinentes tout en améliorant la perception de l'objet
	IplConvKernel *structurant;
/*	structurants possibles : 
		-CV_SHAPE_RECT
    	-CV_SHAPE_CROSS
    	-CV_SHAPE_ELLIPSE
   		-CV_SHAPE_CUSTOM ==> int* à passer dans le paramètre value (dernier param) 
*/    
	structurant = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_ELLIPSE, NULL);
	cvDilate(mask, mask, structurant, 1);
	cvErode(mask, mask, structurant, 1);
	
	cvShowImage("binarisation", mask);
	cvWaitKey (0);
	
	return mask;
}

/**
 * A partir d'une image binaire, calcule et retourne sous forme d'un CvPoint* le barycentre des pixels à 1 (blancs),
 *  cencés représenter l'objet à traquer (aux erreurs de traitement près).
 */
CvPoint * getNewCoord(const IplImage* imgBin)
{
	assert(imgBin->nChannels == 1 );
	
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
			
	CvPoint * bary;
	bary->x = (int)(sommeX / nbPixels);
	bary->y = (int)(sommeY / nbPixels);
	
	return bary;
}

/*
 *	getNewCoord(image binaire,ancienne position) retourne nouvelle position.
 * Méthode envisagée : chercher la zone correspondant à l'objet traquée la plus proche du pixel donnée
 * => méthode non précise
 * \TODO
 */
Pixel * getNewCoord(const IplImage* binaryImg, Pixel * oldPix)
{
	Pixel * c = oldPix;
	
	//Vérif que l'image donnée soit binarisée
	assert (binaryImg->nChannels == 1 );
	
	return c;
}


Pixel * initNaiveColorTrack(IplImage * source, int x, int y)
{
	Pixel pixel;
	CvPoint points;
	points.x = x;
	points.y = y;
	IplImage * hsv;
	
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV);
	
	CvScalar color = cvGet2D(hsv,x,y);
	pixel.points = points;
	pixel.color = color;
	return naiveColorTrack(source,&pixel); 
}


Pixel * naiveColorTrack(IplImage * source, Pixel * clickedPix)
{
	return getNewCoord(binarisation(source, clickedPix), clickedPix);
}

int main(int argc, char* argv[])
{
	IplImage* source = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
	//cvNamedWindow("test", CV_WINDOW_AUTOSIZE);
	cvShowImage("source", source);
	cvWaitKey(0);
	Pixel * pix = initNaiveColorTrack(source, 164,530);
	cout << pix->points.x << "-" << pix->points.y << endl;

	cvDestroyAllWindows ();
	cvReleaseImage (&source);
	
}
