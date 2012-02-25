#ifndef HPP_DRAWINGBOARD
#define HPP_DRAWINGBOARD

/*
	-- Cette classe se charge tout ce qui est affichage du dessin
	-- Elle fait le lien entre WidgetWebcam (donc la trace de mouvements) et le dessin
	-- Classe abstraite, non instanciable. Deux implémentations seront présentes, une pour l'utilisation locale et une pour le réseau.
	-- Dépendante de Qt
*/

#include <iostream>
#include <QGraphicsView>

class DrawingBoard : public QGraphicsView
{
	
	public :
		virtual void drawPoint(int x,int y) = 0;			// Dessine un point
		virtual void drawLine(int fromX,int fromY,int toX,int toY) = 0;	// Dessine une ligne entre deux points
	
	protected :
		QGraphicsScene *scene;	// Représente le dessin lui même
};

#endif
