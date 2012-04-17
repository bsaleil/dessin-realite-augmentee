#ifndef HPP_LIBTRACK
#define HPP_LIBTRACK

/**
 * \file libtrack.hpp
 * \brief Tracking Library header
 * \author Pouer
 * \version r150
 * \date 04/2012
 *
 * Library used to realise an object tracking in a video stream
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cvblob.h>

/**
 * \enum TYPE_TRACK
 * \brief Used to chose the tracking method.
 */
enum TYPE_TRACK {	TRACK_COLOR,/**< For a track based on the object color.*/
					TRACK_SHAPE,/**< For a track based on the object shape.*/
					TRACK_BLOB/**< For a track based on the object.*/
				};

/** 
 * \struct Cursor
 * \brief Structure used to receive and sent data about the track.
 */
typedef struct Cursor
{
	CvPoint center; /**< center pixel of the object area */
	CvPoint cornerA; /**<Up-Left corner of the object area*/
	CvPoint cornerB; /**< Down-Right corner of the object area*/
	unsigned int area; /**< area of the cursor*/
	CvScalar color; /**<HSV color of binarisation*/
	IplImage *mask; /**< mask or template used for tracking.*/
	unsigned int threshold;/**< value of the threshold used for calibration*/
	bool active; /**< determine whether the paint is active or not*/
	TYPE_TRACK flag; /**< type of tracking*/
}Cursor; 


/*------------------------------------------------------------------------------
				Envelope Functions
------------------------------------------------------------------------------*/

/** 
 * \fn Cursor * calibration(IplImage * source, CvPoint A, CvPoint B, TYPE_TRACK flag)
 * \brief Initialize a structure for a TYPE_TRACK Tracking.
 * \param source : Image from which apply the calibration
 * \param A : one of the two pixels defining the object area to track
 * \param B : one of the two pixels defining the object area to track
 * \param flag : determine the tracking method to use.
 * \return A Cursor * structure containing the track informations
 */
Cursor * calibration(IplImage * source, CvPoint A, CvPoint B, TYPE_TRACK flag);

/** 
 * \fn int track(IplImage * source, Cursor * oldCursor)
 * \brief Realize a TYPE_TRACK Tracking, update the struct Cursor.
 * \param source : Image from which apply the track
 * \param oldCursor : structure to update, containing all information
 * \return 0 if success, -1 if failure.
 */				
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
				Color Functions
------------------------------------------------------------------------------*/
/**
 * \fn int binarisation(IplImage * source, Cursor * oldCursor)
 * \brief Update the mask in oldCursor with the source IplImage
 * \param source : The colored source image you want to binarise
 * \param oldCursor : structure to update, containing all information
 * \return 0 if success, -1 if failure.
 *
 * TODO : technical description...
 */		
int binarisation(IplImage * source, Cursor * oldCursor);

//Average color
CvScalar colorAverage(IplImage *hsv,CvPoint A, CvPoint B);

CvScalar sampledColorAverage (IplImage *udrImg, int nbPixels);

CvScalar mainColor(IplImage *hsv, CvPoint A, CvPoint B);

/*------------------------------------------------------------------------------
			    Miscellaneous Functions
------------------------------------------------------------------------------*/
/* Compute the center between point A and B
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
 * 0 success, -1 failure
 */
int setNewCoord(Cursor * oldCursor);

#endif
