#include "libtrack.hpp"
#define MARGE 5
using namespace std;


/*------------------------------------------------------------------------------
				Envelopes Functions
------------------------------------------------------------------------------*/

Cursor * calibration(IplImage * source, CvPoint A, CvPoint B, TYPE_TRACK flag)
{
	if (flag == TRACK_COLOR) 
	{
		return initColorTrack(source, A, B);
	}

	else if (flag == TRACK_SHAPE)
	{
		return initShapeTrack(source, A, B);
	}
	else if (oldCursor->flag == TRACK_BLOB)
	{
		return initBlobTrack(source, oldCursor);
	}
	else return -2;
}

int track(IplImage * source, Cursor * oldCursor)
{
	if (oldCursor->flag == TRACK_COLOR) 
	{
		return colorTrack(source, oldCursor);
	}

	else if (oldCursor->flag == TRACK_SHAPE)
	{
		return shapeTrack(source, oldCursor);
	}

	else if (oldCursor->flag == TRACK_BLOB)
	{
		return 0;// blobTrack(source, oldCursor);
	}
	else return -2;
}

/*------------------------------------------------------------------------------
				Init Functions
------------------------------------------------------------------------------*/

Cursor * initColorTrack(IplImage * source, CvPoint A, CvPoint B) 
{
	Cursor * curs = new Cursor;
	curs->flag = TRACK_COLOR;
	curs->cornerA = A;
	curs->cornerB = B;
	curs->threshold = 10;

	curs->center = center(A,B);
	IplImage * hsv;
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV); //on cree une image hsv copie de source
									
	//TODO calcul moyenne de couleur grace a A et B et hsv;
	CvScalar color = colorAverage(hsv,A,B);
	//CvScalar color = cvGet2D(hsv,curs->center.y,curs->center.x);
	
	cvReleaseImage(&hsv);
	curs->color = color;
	colorTrack(source,curs);
	return curs;
}

Cursor * initShapeTrack(IplImage * source, CvPoint A, CvPoint B) 
{
	Cursor * curs = new Cursor;
	curs->flag = TRACK_SHAPE;
	curs->cornerA = A;
	curs->cornerB = B;
	curs->threshold = 10;

	curs->center = center(A,B);
	CvRect roi;
	roi.x = A.x;
	roi.y = A.y;
	
	roi.width = abs(A.x-B.x);
	roi.height = abs(A.y-B.y);
	
	IplImage * mask = reshape(source, roi);
	curs->mask = mask;		
	
	shapeTrack(source,curs);
	return curs;
}


Cursor initBlobTrack(IplImage * source, CvPoint A, CvPoint B)
{
	Cursor * curs = new Cursor;
	curs->flag = TRACK_COLOR;
	curs->cornerA = A;
	curs->cornerB = B;
	curs->threshold = 10;

	curs->center = center(A,B);
	IplImage * hsv;
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV); //on cree une image hsv copie de source
	CvScalar color = colorAverage(hsv,A,B);
	cvReleaseImage(&hsv);
	curs->color = color;

	colorTrack(source,curs);

	return curs;
}


/*------------------------------------------------------------------------------
				Tracking Functions
------------------------------------------------------------------------------*/
int colorTrack(IplImage * source, Cursor * oldCursor)
{
	int res = binarisation(source, oldCursor);
	if (res == 0) 
	{
		int res2 = setNewCoord(oldCursor);
		return res2;
	}
	else {return -1;}
}

int shapeTrack(IplImage * source, Cursor * oldCursor)
{
	IplImage * result; // If image is W * H and templ is w * h then result must be (W-w+1)* (H-h+1) 
	// Allocate Output Images:
	int iwidth = source->width - oldCursor->mask->width + 1;
	int iheight = source->height - oldCursor->mask->height + 1;
	result= cvCreateImage( cvSize( iwidth, iheight ), 32, 1 );
	cvMatchTemplate(source, oldCursor->mask ,result, /*CV_TM_CCORR*/CV_TM_CCORR_NORMED);
	cvNormalize( result, result, 1, 0, CV_MINMAX );
	
	double minVal, maxVal;
	CvPoint maxLoc, minLoc;
	cvMinMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, NULL); 
	cvReleaseImage(&result);
	int x = maxLoc.x ;
	int y= maxLoc.y ;
	
	//recentrage
	x += (int)(cursor->mask->width /2);
	y += (int)(cursor->mask->height /2);
	
	cursor->center.x =x;
	cursor->center.y =y;
	return 0;
}

/*
int blobTrack(IplImage *source, Cursor * oldCursor);
{
	cvb::CvBlobs blobs;
	IplImage *labelImg=cvCreateImage(cvGetSize(source), IPL_DEPTH_LABEL, 1);
	unsigned int result=cvLabel(oldPix->mask,labelImg,blobs);
	cvRenderBlobs(labelImg,blobs,source,source);
	if ( blobs.begin() != blobs.end())
	{
	int max = blobs.begin()->second->area;
	for (cvb::CvBlobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it)
	{
		if (it->second->area >= max)
		{
			max = it->second->area;
			if (max > 150 &&  (it->second->centroid.x > oldPix->coord.x+10 || it->second->centroid.x < oldPix->coord.x-10) || (it->second->centroid.y > oldPix->coord.y+10 || it->second->centroid.y < oldPix->coord.y-10) )
			{
				oldPix->coord.x = it->second->centroid.x;
				oldPix->coord.y = it->second->centroid.y;
			}
			std::cout << "Blob #" << it->second->label << ": Area=" << it->second->area << ", Centroid=(" << it->second->centroid.x << ", " << it->second->centroid.y << ")" << std::endl;
		}
  		
	}
	
	}
	return 0;
}
*/

/*------------------------------------------------------------------------------
				Colors Functions
------------------------------------------------------------------------------*/

int binarisation(IplImage * source, Cursor *oldPix)
{
	IplImage *hsv;
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV); // on travaille sur l'image en hsv => permet d'ignorer la luminosité
	CvScalar pixel =  oldPix->color; // permet de récupérer la couleur à traquer.
	
	int h = (int)pixel.val[0];
	int s = (int)pixel.val[1];
	//int v = (int)pixel.val[2];
	int tolerance = oldPix->threshold;
	
	IplImage * mask = NULL;
	mask = cvCreateImage(cvGetSize(source), source->depth, 1);
	cvInRangeS(hsv, cvScalar(h - tolerance -1, s - tolerance, 0,0), cvScalar(h + tolerance -1, s + tolerance, 255,0), mask);
	
	//Il convient ensuite d'appliquer une fermeture (érosion puis dilatation) à notre image, 
	//afin d'éliminer les zones non pertinentes tout en améliorant la perception de l'objet
	IplConvKernel *structurant;
/*	structurants possibles : 
	-CV_SHAPE_RECT
    -CV_SHAPE_CROSS
    	-CV_SHAPE_ELLIPSE
   	-CV_SHAPE_CUSTOM ==> int* à passer dans le paramètre value (dernier param) 
*/    
	structurant = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_ELLIPSE, NULL); // cvCreateStructuringElementEx(w,h,x,y,custom)
	cvErode(mask, mask, structurant, 1);
	structurant = cvCreateStructuringElementEx(4, 4, 1, 1, CV_SHAPE_ELLIPSE, NULL);
	cvDilate(mask, mask, structurant, 1);

	cvReleaseImage(&hsv);
	oldPix->mask = mask;
	return 0;
}

CvScalar colorAverage(IplImage *hsv, CvPoint A, CvPoint B)
{
	CvRect roi;
	roi.x = A.x;
	roi.y = A.y;
	
	roi.width = abs(A.x-B.x);
	roi.height = abs(A.y-B.y);
	
	IplImage * mask = reshape(hsv, roi);
	
	
  	CvScalar scalar;
  	
	int h =0;
	int s = 0;
	int v = 0;
	int nbPx =0;

	uchar *p, *line;

  	for (line = (uchar*) mask->imageData; line <  (uchar*) mask->imageData + mask->imageSize; line += mask->widthStep)
  	{
    		for (p = line; p < line + mask->width * mask->nChannels; p+= mask->nChannels)
    			{h+= *p;   nbPx ++;}
		for (p = line+1; p < line + mask->width * mask->nChannels; p+= mask->nChannels)
			s+= *p;   
		for (p = line+2; p < line + mask->width * mask->nChannels; p+= mask->nChannels)
   			v+= *p;   	
  	}  
	cvReleaseImage(&mask);
	scalar.val[0] = h /nbPx;
	scalar.val[1] = s /nbPx ;
	scalar.val[2] = v /nbPx ;
  	return scalar;
}

/*------------------------------------------------------------------------------
			    Miscellaneous Functions
------------------------------------------------------------------------------*/

CvPoint center(CvPoint A, CvPoint B)
{
	CvPoint center;
	center.x = (A.x + B.x)/2;
	center.y = (A.y + B.y)/2;
	return center;
}


IplImage * reshape(IplImage * source, CvRect roi)
{
	IplImage* templ = cvCreateImage(cvSize(roi.width,roi.height), source->depth, source->nChannels);
	cvSetImageROI(source, roi);

	cvCopy(source, templ);
	cvResetImageROI(source);

	return templ;
}

int setNewCoord(Cursor * oldPix)
{
	if(oldPix->mask->nChannels != 1 )
	{
		perror("Invalid IplImage number of channels.");
		return -1;
	}
	
	int sommeX = 0;
	int sommeY = 0;
	int nbPixels = 0;
	for(int x = 0; x < oldPix->mask->width; x++)
		for(int y = 0; y < oldPix->mask->height; y++)
			if(((uchar *)(oldPix->mask->imageData + y*oldPix->mask->widthStep))[x] == 255)
			{
				sommeX += x;
				sommeY += y;
				nbPixels++;
			}
	if (nbPixels > 0)
	{
		int x = (int)(sommeX / nbPixels);
		int y = (int)(sommeY / nbPixels);
		
		// On ne redessine le point que si il y a une différence d'au moins X pixels
		if (x>oldPix->center.x+MARGE || x<oldPix->center.x-MARGE)
			oldPix->center.x = (int)(sommeX / nbPixels);
		if (y>oldPix->center.y+MARGE || y<oldPix->center.y-MARGE)
			oldPix->center.y = (int)(sommeY / nbPixels);
	}
	
	return 0;
}
