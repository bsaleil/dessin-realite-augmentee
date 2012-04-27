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
#include <QGraphicsItem>
#include <QPen>
#include <QGLWidget>
#include <QDebug>
#include <QGraphicsProxyWidget>

#include "gesturewidget.hpp"

class DrawingBoard : public QGraphicsView
{	
	Q_OBJECT	
	
	public :
		virtual void drawQPoint(QPoint) = 0;
		virtual void drawPoint(int x,int y) = 0;			// Dessine un point
		virtual void drawLine(int fromX,int fromY,int toX,int toY) = 0;	// Dessine une ligne entre deux points
		virtual void flushScene() = 0;					// Vide la scène
		
		virtual void setFirstPoint(bool f);	// Change l'état du pooint à dessiner
		virtual void initCursor();		// Initialise le curseur
		virtual void initGestureUI();		// Initialise l'interface gestuelle
		virtual void moveCursor(int x, int y);	// Met a jour la position du curseur
		
		virtual void init();			// Initialise l'ensemble du tableau
		virtual void showGestureWidget(bool);
	
	public slots :
		virtual void changeWidth(int width);	// Change l'épaisseur du curseur
		virtual void changeColor(QColor color);	// Change la couleur du curseur

	protected :
		QGraphicsPixmapItem *cursor; // Représente le curseur
		QGraphicsScene *scene;	// Représente le dessin lui même
		QPen *pen;		// Représente l'état du pinceau
		QPoint *precedent;	// Représente le point précédemment tracé
		bool firstPoint;	// Premier point (utilisé pour les lignes)
		GestureWidget *gestureWidget; // Interface gestuelle
		QGraphicsProxyWidget *proxyGestureWidget; // Proxy du widget de l'IG
};

#endif
