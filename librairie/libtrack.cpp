#include "libtrack.hpp"
using namespace std;

Cursor * calibration(IplImage * source, CvPoint A, CvPoint B, Type_Track flag)
{
	if (flag == TRACK_COLOR) 
	{
		return initColorTrack(source, A, B);
	}

	else if (flag == TRACK_SHAPE)
	{
		return 0; // TODO initShapeTrack
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
		return 0; // TODO initShapeTrack
	}
}

/*
Initialise un track par couleur
Entrée: Une image, la zone du curseur
Retour: La structure representant le curseur
*/
Cursor * initColorTrack(IplImage * source, CvPoint A, CvPoint B) 
{
	Cursor  curs;
	curs.flag = TRACK_COLOR;
	curs.cornerA = A;
	curs.cornerB = B;
	curs.threshold = 10;
	//TODO calcul du centre en fonction de A et de B 
	cout << "ok" << endl;
	curs.center = center(A,B);
	IplImage * hsv;
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV); //on cree une image hsv copie de source
									
	//TODO calcul moyenne de couleur grace a A et B et hsv;
	//CvScalar color = colorAverage(hsv,A,B);
	CvScalar color = cvGet2D(hsv,curs.center.y,curs.center.x);
	
	cvReleaseImage(&hsv);
	curs.color = color;
	colorTrack(source,&curs); 
	return &curs;
}

// Retourne l'image binarisée de 'source' en fonction des informations contenues dans le 'oldCursor' (coord et coul)
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

	cvReleaseImage(&hsv);
	oldPix->mask = mask;
	return 0;
}

/*Met à jour le Cursor représentant le barycentre du curseur à tracker.
 * 0 en cas de succès
 * -1 en cas d'erreur : à définir
 */
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
		oldPix->center.x = (int)(sommeX / nbPixels);
		oldPix->center.y = (int)(sommeY / nbPixels);
	}
	
	return 0;
}




/*
Track la nouvelle position du curseur sur l'image par couleur par moyenne de pixel
Entrée: Une image et un curseur
Retour: une int (et maj le curseur)
*/
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

/*
//Initialise la structure de suivi naif par Couleur
Cursor initBlobTrack(IplImage * source, CvPoint A, CvPoint B)
{
	Cursor * cursor;
	CvPoint points;
	cursor->cornerA= A;
	cursor->cornerB= B;
	//TODO calcul du centre en fonction de A et de B 
	cursor->center = center(A,B);
	IplImage * hsv;
	hsv = cvCloneImage(source);
	cvCvtColor(source, hsv, CV_BGR2HSV); //on cree une image hsv copie de source
									
	//TODO calcul moyenne de couleur grace a A et B et hsv;
	CvScalar color = colorAverage(hsv,A,B);
	cvReleaseImage(&hsv);
	cursor->center = points;
	cursor->color = color;
	blobTrack(source,cursor); 
	return cursor;
}


//Met à jour la structure de suivi
int blobTrack(IplImage * source, Cursor * oldPix)
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
/*
Track la nouvelle position du curseur sur l'image par forme
Entrée: Une image et un curseur
Retour: une int (et maj le curseur)
*/
int shapeTrack(IplImage * source, IplImage * cursor)
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

//Average color
/*CvScalar colorAverage(IplImage *hsv,CvPoint A, CvPoint B)
{
  	CvScalar scalar;
  	
	int h =0;
	int s = 0;
	int nbPx =0;

	uchar *p, *line;

  for (line =  (uchar*) hsv->imageData;
       line <  (uchar*) hsv->imageData + hsv->imageSize;
       line += hsv->widthStep)
  {
    for (p = line; p < line + hsv->width * hsv->nChannels; p+=nbChannels)
    	{h+= *p;   nbPx ++;}
    for (p = line; p < line + hsv->width * hsv->nChannels; p+=nbChannels)
    	s+= *p;   
  }
  scalar->val0 = h;
  scalar->val1 = s;
  
  return scalar;
}*/

//Center between A & B
CvPoint center(CvPoint A, CvPoint B)
{
	CvPoint center;
	center.x = abs(A.x - B.x);
	center.y = abs(A.y - B.y);
	return center;
}
