#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


typedef struct Coordonnees
{
    int x; // Abscisses
    int y; // Ordonnées
}Coord; // Utiliser de cvScalar plutôt je pense?

IplImage * Binarisation(IplImage * source, int x, int y, CvScalar& oldPixel); // si on passe NULL, on prend le pixel sous x et y pour capturer la couleur

/*
 *  getNewCoord(image binaire,ancienne position) => retourne nouvelle position.
 */
Coord getNewCoord(const IplImage* binaryImg, Coord oldCoord);

int * NaifColorTrack(IplImage * source, int x, int y); 
