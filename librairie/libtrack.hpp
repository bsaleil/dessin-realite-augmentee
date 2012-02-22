#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


typedef struct Coordonnees
{
    int x; // Abscisses
    int y; // Ordonnées
}Coord;

IplImage * Binarisation(IplImage * source, int x, int y);

/*
 *  getNewCoord(image binaire,ancienne position) => retourne nouvelle position.
 */
Coord getNewCoord(const IplImage* binaryImg, Coord oldCoord);

int * NaifColorTrack(IplImage * source, int x, int y); 
