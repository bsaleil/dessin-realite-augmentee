#include "libtrack.hpp"
#define MARGE 5
#define GROW 6
#define RATIO 1.5
#define SLICESIZE 10
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
	else if (flag == TRACK_BLOB)
	{
		return initBlobTrack(source, A, B);
	}
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
		return blobTrack(source, oldCursor);
	}
	else return -2;
}

/*------------------------------------------------------------------------------
				Init Functions
------------------------------------------------------------------------------*/

Cursor * initBlobTrack(IplImage * source, CvPoint A, CvPoint B)
{
	Cursor * curs = new Cursor;
	curs->flag = TRACK_BLOB;
	curs->cornerA = A;
	curs->cornerB = B;
	curs->area = abs(A.x-B.x)*abs(A.y-B.y);

	curs->threshold = 10;
	curs->active = false;
	curs->center = center(A,B);
	IplImage * hsv;
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV); //on cree une image hsv copie de source
	CvScalar color = mainColor(hsv,A,B);
	cvReleaseImage(&hsv);
	//CvScalar color = cvGet2D(hsv,curs->center.y,curs->center.x);
	curs->color = color;

	IplImage * clone;
	clone = cvCloneImage(source);
	blobTrack(clone,curs);
	cvReleaseImage(&clone);

	return curs;
}


Cursor * initColorTrack(IplImage * source, CvPoint A, CvPoint B) 
{
	Cursor * curs = new Cursor;
	curs->flag = TRACK_COLOR;
	curs->cornerA = A;
	curs->cornerB = B;
	curs->threshold = 10;
	curs->active = true;
	curs->center = center(A,B);
	IplImage * hsv;
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV); //on cree une image hsv copie de source

	//CvScalar color = cvGet2D(hsv,curs->center.y,curs->center.x);
	CvScalar color = colorAverage(hsv,A,B);
	cvReleaseImage(&hsv);

	curs->color = color;
	curs->area =0;

	colorTrack(source,curs);

	for(int x = 0; x < curs->mask->width; x++)
		for(int y = 0; y < curs->mask->height; y++)
			if(((uchar *)(curs->mask->imageData + y*curs->mask->widthStep))[x] == 255)
			{
				curs->area++;
			}

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
	roi.x = min(A.x, B.x);
	roi.y = min(A.y, B.y);
	
	roi.width = abs(A.x-B.x);
	roi.height = abs(A.y-B.y);
	
	IplImage * mask = reshape(source, roi);
	curs->mask = mask;		
	
	shapeTrack(source,curs);
	return curs;
}




/*------------------------------------------------------------------------------
				Tracking Functions
------------------------------------------------------------------------------*/

int blobTrack(IplImage *source, Cursor * oldCursor)
{
	
	int res = binarisation(source, oldCursor);
	if (res == 0) 
	{
		int res2 = blobFounding(source,oldCursor);
		return res2;
	}
	else {return -1;}
}

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
	x += (int)(oldCursor->mask->width /2);
	y += (int)(oldCursor->mask->height /2);
	
	oldCursor->center.x =x;
	oldCursor->center.y =y;
	return 0;
}


/*------------------------------------------------------------------------------
				Colors Functions
------------------------------------------------------------------------------*/

int binarisation(IplImage *source, Cursor *oldCursor)
{
	IplImage *hsv;
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV); // on travaille sur l'image en hsv => permet d'ignorer la luminosité
	CvScalar pixel =  oldCursor->color; // permet de récupérer la couleur à traquer.
	
	int h = (int)pixel.val[0];
	int s = (int)pixel.val[1];
	//int v = (int)pixel.val[2];
	int tolerance = oldCursor->threshold;
	
	IplImage * mask = NULL;
	mask = cvCreateImage(cvGetSize(source), source->depth, 1);
	cvInRangeS(hsv, cvScalar(h - (tolerance/RATIO) -1, s - tolerance, 0,0), cvScalar(h + (tolerance/RATIO) -1, s + tolerance, 255,0), mask);
	
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
	cvReleaseStructuringElement(&structurant);
	oldCursor->mask = mask;
	return 0;
}

CvScalar colorAverage(IplImage *hsv, CvPoint A, CvPoint B)
{
	CvRect roi;
	roi.x = min(A.x, B.x);
	roi.y = min(A.y, B.y);
	
	roi.width = abs(A.x-B.x);
	roi.height = abs(A.y-B.y);
	
	IplImage * mask = reshape(hsv, roi);
	
	//création sous image pour la zone "pertinente" de l'objet
	//int ratio = 20;
	//CvRect underRect = underROI(roi, ratio); // ratio a determiner/fixer : 20% ?
	//IplImage * suitableZone = reshape(hsv, underRect);
	
	//puis on on fait la moyenne sur un échantillonage de cette sous-zone, afin de servir de seuil
	// int nbPixels = 10;
	//CvScalar underAvg =  sampledColorAverage (suitableZone, nbPixels); // nbPixels a determiner/fixer : 10/20 pixels.
	//puis on peut libérer cette sous image
	//cvReleaseImage(&suitableZone);
	
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
	cout << scalar.val[0] << ":" << scalar.val[1] << endl;
  	return scalar;
}
  	
CvScalar mainColor(IplImage *hsv, CvPoint A, CvPoint B)
{
	CvRect roi;
	roi.x = min(A.x, B.x);
	roi.y = min(A.y, B.y);
	
	roi.width = abs(A.x-B.x);
	roi.height = abs(A.y-B.y);
	
	IplImage * mask = reshape(hsv, roi);
	
  	CvScalar scalar;
  	
  	int h =0;
	int s = 0;
	int v = 0;
	
	int histo[180/SLICESIZE]= {0};
	int index=0;
	int max = histo[index];
	int nbPixels =0;
	uchar *p, *line, *tmp;
	
  	for (line = (uchar*) mask->imageData; line <  (uchar*) mask->imageData + mask->imageSize; line += mask->widthStep)
  	{
    		for (p = line; p < line + mask->width * mask->nChannels; p+= mask->nChannels)
    		{
    			histo[*p/SLICESIZE]++;
    		} 
	}
	
	for (int i =1; i<180/SLICESIZE; i++)
	{
		if (histo[i]>max)
		{
			max = histo[i];
			index =i;
		}	
	}
	
	int val = index*SLICESIZE; // juste pour test
	for (line = (uchar*) mask->imageData; line <  (uchar*) mask->imageData + mask->imageSize; line += mask->widthStep)
  	{
		for (p = line+1; p < line + mask->width * mask->nChannels; p+= mask->nChannels)
		{
			tmp = p-1;
			if (*tmp < val+(SLICESIZE/2) && *tmp > val-(SLICESIZE/2)) 
			{	s+= *p;
				h+= *tmp;
				nbPixels ++;
			}
		}
	}
	
	scalar.val[0] = h/nbPixels;
	scalar.val[1] = s/nbPixels;
	scalar.val[2] = v;
	cout << scalar.val[0] << ":" << scalar.val[1] << endl;
	return scalar;
}
CvScalar sampledColorAverage (IplImage *udrImg, int nbPixels)
{
	CvScalar moy;

	int h =0;
	int s = 0;
	int v = 0;
	
	srand ( time(NULL) );
	
	int x,y;
	CvScalar randPx;
	for (int i =0 ; i < nbPixels ; i++)
	{
		x = rand() % udrImg->width;
		y = rand() % udrImg->height;
		randPx = cvGet2D(udrImg, x,y);
		h += randPx.val[0];
		s += randPx.val[1];
		v += randPx.val[2];
	}

	moy.val[0] = h /nbPixels;
	moy.val[1] = s /nbPixels ;
	moy.val[2] = v /nbPixels ;
  	return moy;
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

int blobFounding(IplImage *source, Cursor * oldCursor)
{
	cvb::CvBlobs blobs;
	IplImage *labelImg=cvCreateImage(cvGetSize(source), IPL_DEPTH_LABEL, 1);
	cvLabel(oldCursor->mask,labelImg,blobs);
	if ( blobs.begin() != blobs.end())
	{
	cvb::CvBlobs::const_iterator closest;
	cvb::CvBlobs::const_iterator it;
	
	int difference = 0;
	int reference = abs(oldCursor->area - blobs.begin()->second->m00);

	closest = blobs.begin();
	for (it=blobs.begin(); it!=blobs.end(); ++it)
	{
		difference = abs(oldCursor->area - it->second->m00);
		
		if (difference < reference)
		{
			reference = difference;
			closest = it;
		}
	}
	if (closest->second->m00 < oldCursor->area * 10 && closest->second->m00 > oldCursor->area/10)
	{
	cvRenderBlob(labelImg,closest->second,source,source);
	if ((closest->second->centroid.x > oldCursor->center.x+MARGE || closest->second->centroid.x < oldCursor->center.x-MARGE) || (closest->second->centroid.y > oldCursor->center.y+MARGE || closest->second->centroid.y < oldCursor->center.y-MARGE) )
	{

		oldCursor->center.x = closest->second->centroid.x;
		oldCursor->center.y = closest->second->centroid.y;

		//oldCursor->area = closest->second->m00;
	}
	if (closest->second->m00 > (oldCursor->area+(oldCursor->area/GROW)))
	{
		oldCursor->active = true;
	}

	else if (closest->second->m00 < (oldCursor->area+(oldCursor->area/(GROW*2))))
	{
		oldCursor->active = false;
	}
	}
	}
	cvReleaseImage(&labelImg);
	
	cvReleaseBlobs(blobs);
	return 0;
}

IplImage * reshape(IplImage * source, CvRect roi)
{
	IplImage* templ = cvCreateImage(cvSize(roi.width,roi.height), source->depth, source->nChannels);
	cvSetImageROI(source, roi);

	cvCopy(source, templ);
	cvResetImageROI(source);

	return templ;
}

CvRect underROI (CvRect fullRect, int ratio)
{
	CvRect underRect;
	underRect.x = fullRect.x + fullRect.width/2 - (ratio / 200)* fullRect.width;
	underRect.y = fullRect.y + fullRect.height/2 - (ratio / 200)* fullRect.height;
	underRect.width = (ratio / 100)* fullRect.width;
	underRect.height = (ratio / 100)* fullRect.height;
	
	return underRect;
}

int setNewCoord(Cursor * oldCursor)
{
	if(oldCursor->mask->nChannels != 1 )
	{
		perror("Invalid IplImage number of channels.");
		return -1;
	}
	
	int sommeX = 0;
	int sommeY = 0;
	int nbPixels = 0;
	for(int x = 0; x < oldCursor->mask->width; x++)
		for(int y = 0; y < oldCursor->mask->height; y++)
			if(((uchar *)(oldCursor->mask->imageData + y*oldCursor->mask->widthStep))[x] == 255)
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
		if (x>oldCursor->center.x+MARGE || x<oldCursor->center.x-MARGE)
			oldCursor->center.x = (int)(sommeX / nbPixels);
		if (y>oldCursor->center.y+MARGE || y<oldCursor->center.y-MARGE)
			oldCursor->center.y = (int)(sommeY / nbPixels);
	}

	if (nbPixels > (oldCursor->area+(oldCursor->area/GROW)))
	{
		oldCursor->active = true;
	}

	else if (nbPixels < (oldCursor->area+(oldCursor->area/(GROW*2))))
	{
		oldCursor->active = false;
	}
	
	return 0;
}
