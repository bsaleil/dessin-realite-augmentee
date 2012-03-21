#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

enum Type_Track {TRACK_COLOR, TRACK_SHAPE, TRACK_BLOB};

typedef struct Cursor
{
    CvPoint center; // Représente le centre de l'objet à suivre (à maj)
    CvPoint cornerA; // premier des deux coins formant le cadre de l'objet
    CvPoint cornerB; // second des deux coins formant le cadre de l'objet
    CvScalar color; // couleur moyenne de l'objet à suivre
    IplImage* mask; // représente soit le mask soit la sous-image, suivant le type de track
    unsigned char threshold; //seuil ou tolérance pour le track (pas sur du type, permet de borner [0,255])
		//TODO Ajouter les infos blob (AREA, CENTROID etc)
}Cursor; 

// Retourne l'image binarisée de 'source' en fonction des informations contenues dans le 'oldCursor'
IplImage * binarisation(IplImage * source, Cursor * oldCursor);

/* Met à jour le Cursor représentant le barycentre du curseur à tracker.
 * 0 en cas de succès
 * -1 en cas d'erreur : à définir
 */
int setNewCoord(const IplImage* binaryImg, Cursor * oldPix);

//Initialise la structure de suivi par Couleur
Cursor initColorTrack(IplImage * source, int x, int y);

//Met à jour la structure de suivi par Couleur en fonction de l'image passée en param.
int colorTrack(IplImage * source, Cursor * oldPix);

//Initialise la structure de suivi par matching de forme&couleur
//IplImage initShapeTrack(IplImage * source, CvPoint a, CvPoint b); //TODO

//Met à jour la structure de suivi par forme
int shapeTrack(IplImage * source, IplImage * cursor);
