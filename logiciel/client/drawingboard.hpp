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
#include <QPen>

class DrawingBoard : public QGraphicsView
{	
	Q_OBJECT	
	
	public :
		virtual void drawQPoint(QPoint) = 0;
		virtual void drawPoint(int x,int y) = 0;			// Dessine un point
		virtual void drawLine(int fromX,int fromY,int toX,int toY) = 0;	// Dessine une ligne entre deux points
		virtual void flushScene() = 0;					// Vide la scène
	
	public slots :
		virtual void changeWidth(int width)
		{
			pen->setWidth(width);
		}
		
		virtual void changeColor(QColor color)
		{
			pen->setColor(color);
		}

	protected :
		QGraphicsScene *scene;	// Représente le dessin lui même
		QPen *pen;		// Représente l'état du pinceau
		QPoint *precedent;	// Représente le point précédemment tracé
		bool firstPoint;	// Premier point (utilisé pour les lignes)
};

#endif
