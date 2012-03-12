#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cvblob.h>



typedef struct Cursor
{
    CvPoint coord; // coordonnées
    CvScalar color; // couleur
    //éventuellement ajouter de nouveaux attributs par la suite, genre un IplImage qui représente la forme de l'objet
    IplImage structure; //?
}Cursor; 

// Retourne l'image binarisée de 'source' en fonction des informations contenues dans le 'oldCursor'
IplImage * binarisation(IplImage * source, Cursor * oldCursor);

//Initialise la structure de suivi naif par Couleur
Cursor initBlobTrack(IplImage * source, int x, int y);

//Met à jour la structure de suivi naif par Couleur en fonction de l'image passée en param.
int blobTrack(IplImage * source, Cursor * oldPix);
