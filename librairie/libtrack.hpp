#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


typedef struct Cursor
{
    CvPoint coord; // coordonnées
    CvScalar color; // couleur
    //éventuellement ajouter de nouveaux attributs par la suite, genre un IplImage qui représente la forme de l'objet
    IplImage structure; //?
}Cursor; 

// Retourne l'image binarisée de 'source' en fonction des informations contenues dans le 'oldCursor'
IplImage * binarisation(IplImage * source, Cursor * oldCursor);

/*
 *  getNewCoord(image binaire,ancienne position) => retourne nouvelle position.
 */
CvPoint * getNewCoord(const IplImage* binaryImg, Cursor * oldCursor);

/* Met à jour le Cursor représentant le barycentre du curseur à tracker.
 * 0 en cas de succès
 * -1 en cas d'erreur : à définir
 */
int setNewCoord(const IplImage* binaryImg, Cursor * oldPix);

//Initialise la structure de suivi naif par Couleur
Cursor initNaiveColorTrack(IplImage * source, int x, int y);

//Met à jour la structure de suivi naif par Couleur en fonction de l'image passée en param.
int naiveColorTrack(IplImage * source, Cursor * oldPix);

//Initialise la structure de suivi par matching de forme&couleur
IplImage initNaiveShapeTrack(IplImage * source, CvPoint a, CvPoint b);

//Met à jour la structure de suivi par forme
int naiveShapeTrack(IplImage * source, IplImage * cursor);
