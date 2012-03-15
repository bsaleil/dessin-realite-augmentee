#include "libtrack.hpp"
using namespace std;

// Retourne l'image binarisée de 'source' en fonction des informations contenues dans le 'oldCursor' (coord et coul)
IplImage * binarisation(IplImage * source, Cursor *oldPix)
{
	IplImage *hsv;
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV); // on travaille sur l'image en hsv => permet d'ignorer la luminosité
	CvScalar pixel =  oldPix->color; // permet de récupérer la couleur à traquer.
	
	int h = (int)pixel.val[0];
	int s = (int)pixel.val[1];
	//int v = (int)pixel.val[2];
	int tolerance = 14; // à passer en paramètre d'ajustement??
	
	IplImage * mask = NULL;
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
	structurant = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_ELLIPSE, NULL); // la le 5,5 représente le structurant (kernel) utilisé pour l'ouverture
	cvErode(mask, mask, structurant, 1);
	structurant = cvCreateStructuringElementEx(4, 4, 1, 1, CV_SHAPE_ELLIPSE, NULL);
	cvDilate(mask, mask, structurant, 1);
	
	//cvShowImage("binarisation", mask);
	//cvWaitKey (0);
	cvReleaseImage(&hsv);
	//cvReleaseImage(&source);
	return mask;
}

/*Met à jour le Cursor représentant le barycentre du curseur à tracker.
 * 0 en cas de succès
 * -1 en cas d'erreur : à définir
 */
int setNewCoord(const IplImage* imgBin, Cursor * oldPix)
{
	if(imgBin->nChannels != 1 )
	{
		perror("Invalid IplImage number of channels.");
		return -1;
	}
	
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
	if (nbPixels > 0)
	{
		oldPix->coord.x = (int)(sommeX / nbPixels);
		oldPix->coord.y = (int)(sommeY / nbPixels);
	}
	
	return 0;
}


/*
Initialise un track naif par couleur
Entrée: Une image, la zone du curseur
Retour: La structure representant le curseur
*/
Cursor initNaiveColorTrack(IplImage * source, int x, int y) //TODO passer en ZONE au lieu d'en point
{
	Cursor pixel;
	CvPoint points;
	points.x = x;
	points.y = y;
	IplImage * hsv;
	
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV);
	
	CvScalar color = cvGet2D(hsv,y,x);
	cvReleaseImage(&hsv);
	pixel.coord = points;
	pixel.color = color;
	naiveColorTrack(source,&pixel); 
	return pixel;
}

/*
Track la nouvelle position du curseur sur l'image par couleur par moyenne de pixel
Entrée: Une image et un curseur
Retour: une int (et maj le curseur)
*/
int naiveColorTrack(IplImage * source, Cursor * clickedPix)
{
	IplImage *mask = binarisation(source, clickedPix);
	int res = setNewCoord(mask, clickedPix);
	cvReleaseImage(&mask);
	return res;
}
/*
Track la nouvelle position du curseur sur l'image par forme
Entrée: Une image et un curseur
Retour: une int (et maj le curseur)
*/
int naiveShapeTrack(IplImage * source, IplImage * cursor)
{
	IplImage * result; // If image is W * H and templ is w * h then result must be (W-w+1)* (H-h+1) 
	// Allocate Output Images:
	int iwidth = source->width - cursor->width + 1;
	int iheight = source->height - cursor->height + 1;
	result= cvCreateImage( cvSize( iwidth, iheight ), 32, 1 );
	cvMatchTemplate(source, cursor ,result, /*CV_TM_CCORR*/CV_TM_CCORR_NORMED);
	cvNormalize( result, result, 1, 0, CV_MINMAX );
	cvShowImage("result", result); 
	cvWaitKey(0);
	
	double minVal, maxVal;
	CvPoint maxLoc, minLoc;
	cvMinMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, NULL); 
	cvReleaseImage(&result);
	int x = maxLoc.x ;
	int y= maxLoc.y ;
	
	//recentrage
	x += (int)(cursor->width /2);
	y += (int)(cursor->height /2);
	
	cout << x << "--" << y << endl;
	return 0;
}
