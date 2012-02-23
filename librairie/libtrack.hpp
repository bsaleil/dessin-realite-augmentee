#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


typedef struct Pixel
{
    CvPoint points; // coordonnées
    CvScalar color; // Couleur
    //éventuellement ajouter de nouveaux attributs par la suite.
}Pixel; 

// Retourne l'image binarisée de 'source' en fonction des informations contenues dans le 'oldPixel'
IplImage * binarisation(IplImage * source, Pixel * oldPix);

/*
 *  getNewCoord(image binaire,ancienne position) => retourne nouvelle position.
 */
Pixel * getNewCoord(const IplImage* binaryImg, Pixel * oldPix);

Pixel * initNaiveColorTrack(IplImage * source, int x, int y);
Pixel * naiveColorTrack(IplImage * source, Pixel * oldPix);
