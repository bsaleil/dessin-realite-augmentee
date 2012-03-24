#ifndef HPP_LIBTRACK
#define HPP_LIBTRACK

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

enum Type_Track {TRACK_COLOR, TRACK_SHAPE, TRACK_BLOB};

typedef struct Cursor
{
	CvPoint center; // center position
	CvPoint cornerA; //Up-Left position
	CvPoint cornerB; // Down-Right

	CvScalar color; //HSV color of binarisation
	IplImage *mask; // mask or template used for tracking.
	unsigned int threshold;

	Type_Track flag; // type of tracking
}Cursor; 

Cursor * calibration(IplImage * source, CvPoint A, CvPoint B, Type_Track flag);

int track(IplImage * source, Cursor * oldCursor);

// Update the mask in oldCursor with the source IplImage
int binarisation(IplImage * source, Cursor * oldCursor);

/* Met à jour le Cursor représentant le barycentre du curseur à tracker.
 * 0 en cas de succès
 * -1 en cas d'erreur : à définir
 */
int setNewCoord(const IplImage* binaryImg, Cursor * oldPix);

//Initialise la structure de suivi naif par Couleur
Cursor * initColorTrack(IplImage * source, CvPoint A, CvPoint B);

Cursor * initShapeTrack(IplImage * source, CvPoint A, CvPoint B);

//Met à jour la structure de suivi naif par Couleur en fonction de l'image passée en param.
int colorTrack(IplImage * source, Cursor * oldPix);

//Initialise la structure de suivi par matching de forme&couleur
//IplImage initShapeTrack(IplImage * source, CvPoint a, CvPoint b); //TODO

//Met à jour la structure de suivi par forme
int shapeTrack(IplImage *source, Cursor * cursor);

//Average color
CvScalar colorAverage(IplImage *hsv,CvPoint A, CvPoint B);

//Center between A & B
CvPoint center(CvPoint A, CvPoint B);

Cursor * cloneCursor(Cursor * C);

IplImage * reshape(IplImage * source, CvRect roi);

#endif
