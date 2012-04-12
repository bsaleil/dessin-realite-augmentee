#ifndef HPP_LIBTRACK
#define HPP_LIBTRACK

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cvblob.h>

enum TYPE_TRACK {TRACK_COLOR, TRACK_SHAPE, TRACK_BLOB};

typedef struct Cursor
{
	CvPoint center; // center position
	CvPoint cornerA; //Up-Left position
	CvPoint cornerB; // Down-Right
	unsigned int area; // area of the cursor
	CvScalar color; //HSV color of binarisation
	IplImage *mask; // mask or template used for tracking.
	unsigned int threshold;
	bool active;

	TYPE_TRACK flag; // type of tracking
}Cursor; 


/*------------------------------------------------------------------------------
				Envelopes Functions
------------------------------------------------------------------------------*/

/* Initialise a TYPE_TRACK Tracking
 * Return : A Cursor * structure containing the track informations*/				
Cursor * calibration(IplImage * source, CvPoint A, CvPoint B, TYPE_TRACK flag);

/* Initialise a TYPE_TRACK Tracking
 * Return : 0 if success, -1 if failure. Also update oldCursor.*/				
int track(IplImage * source, Cursor * oldCursor);

/*------------------------------------------------------------------------------
				Init Functions
------------------------------------------------------------------------------*/
Cursor * initColorTrack(IplImage * source, CvPoint A, CvPoint B);
Cursor * initBlobTrack(IplImage * source, CvPoint A, CvPoint B);
Cursor * initShapeTrack(IplImage * source, CvPoint A, CvPoint B);


/*------------------------------------------------------------------------------
				Tracking Functions
------------------------------------------------------------------------------*/
int colorTrack(IplImage * source, Cursor * oldCursor);
int blobTrack(IplImage *source, Cursor * oldCursor);
int shapeTrack(IplImage *source, Cursor * oldCursor);

/*------------------------------------------------------------------------------
				Colors Functions
------------------------------------------------------------------------------*/
/*Update the mask in oldCursor with the source IplImage
Return : 0 if success, -1 if failure. Also update oldCursor.*/		
int binarisation(IplImage * source, Cursor * oldCursor);

//Average color
CvScalar colorAverage(IplImage *hsv,CvPoint A, CvPoint B);

CvScalar sampledColorAverage (IplImage *udrImg, int nbPixels);

/*------------------------------------------------------------------------------
			    Miscellaneous Functions
------------------------------------------------------------------------------*/
/* Compute the center beteween point A and B
 * CvPoint center
 */
CvPoint center(CvPoint A, CvPoint B);

/* Update the center of the cursor by blob founding
 * CvPoint center
 */
int blobFounding(IplImage * source,Cursor * oldCursor);
/* Return source cropped by cvrect roi
 * IplImage * cropped
 */
IplImage * reshape(IplImage * source, CvRect roi);

CvRect underROI (CvRect fullRect, int ratio);

/* Update the center variable in oldCursor
 * 0 success
 * -1 failure
 */
int setNewCoord(Cursor * oldCursor);

#endif
