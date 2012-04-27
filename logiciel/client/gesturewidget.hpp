#ifndef HPP_GESTUREWIDGET
#define HPP_GESTUREWIDGET

/*
	-- Cette classe construit l'interface gestuelle
	-- Dépendante de Qt
*/

#include <iostream>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QPen>
#include <QLabel>

#include "gesturebutton.hpp"

using namespace std;

class GestureWidget : public QWidget
{

	public :
		GestureWidget(bool*,QPen*);
		// Donne la position de la souris
		void mousePosition(int x, int y);
		// Permet de modifier la couleur du bouton d'état de l'extérieur
		void changeButtonColor(QColor);
	
	private :	
		// Boutons d'indication
		GestureButton *buttonColor;
		GestureButton *labelWidth;
	
		// Boutons pour les couleurs
		GestureButton *buttonRed;
		GestureButton *buttonGreen;
		GestureButton *buttonBlue;
		GestureButton *buttonBlack;
		
		// Bouton pour la gomme
		GestureButton *buttonEraser;
		
		// Bouton pour la taille du pinceau
		GestureButton *buttonMinus;
		GestureButton *buttonPlus;
		
		// Etat de la classe drawing board
		bool *firstPoint;
		QPen *pen;
};

#endif
