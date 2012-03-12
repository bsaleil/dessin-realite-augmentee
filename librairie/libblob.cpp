#include "libblob.hpp"

IplImage * binarisation(IplImage * source, Cursor *oldPix)
{
	IplImage *hsv;
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV); // on travaille sur l'image en hsv => permet d'ignorer la luminosité
	CvScalar pixel =  oldPix->color; // permet de récupérer la couleur à traquer.
	
	int h = (int)pixel.val[0];
	int s = (int)pixel.val[1];
	int v = (int)pixel.val[2];
	int tolerance = 14; // à passer en paramètre d'ajustement??
	
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

//Initialise la structure de suivi naif par Couleur
Cursor initBlobTrack(IplImage * source, int x, int y)
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
	blobTrack(source,&pixel); 
	return pixel;
}


//Met à jour la structure de suivi naif par Couleur en fonction de l'image passée en param.
int blobTrack(IplImage * source, Cursor * oldPix)
{
	cvb::CvBlobs blobs;
	IplImage *labelImg=cvCreateImage(cvGetSize(source), IPL_DEPTH_LABEL, 1);
	IplImage *mask = binarisation(source, oldPix);
	cvShowImage("Live",mask);
	cvWaitKey(0);
	unsigned int result=cvLabel(mask,labelImg,blobs);
	cvRenderBlobs(labelImg,blobs,source,source);
	cvShowImage("Live",labelImg);
	cvWaitKey(0);

	cvShowImage("Live",source);
	cvWaitKey(0);
}

int main(int argc, char* argv[])
{
        IplImage* source = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
	int x = atoi(argv[2]);
	int y = atoi(argv[3]);
        cvShowImage("source", source);
        cvWaitKey(0);
	initBlobTrack(source, x, y);
        /* Destruction de la fenêtre */
        cvDestroyAllWindows ();
        /* Libération de la mémoire */
        cvReleaseImage (&source);
        
}
