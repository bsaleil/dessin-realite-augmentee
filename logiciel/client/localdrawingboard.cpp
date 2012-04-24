#include "localdrawingboard.hpp"

LocalDrawingBoard::LocalDrawingBoard()
{

	// Création du pinceau par défaut
	pen = new QPen();
		pen->setWidth(5);
		pen->setColor(QColor("#000000"));
		
	setFixedSize(645, 485);
	setSceneRect(0, 0, 640, 480);
	scene = new QGraphicsScene();
	setScene(scene);
	
	// Savoir si c'est le premier point
	firstPoint = true;
	
	// On initialise le curseur
	initCursor();
}

void LocalDrawingBoard::drawPoint(int x, int y)
{	
	if(firstPoint == true)
	{
		scene->addEllipse(x, y, pen->width(), pen->width(), *pen, QBrush(Qt::SolidPattern));
		precedent = new QPoint(x, y);
		firstPoint = false;
	}
	else
	{
		drawLine(precedent->x(), precedent->y(), x, y);
		precedent->setX(x);
		precedent->setY(y);
	}
}

// Dessine un point sur le tableau
void LocalDrawingBoard::drawQPoint(QPoint p)
{	
	
	if(firstPoint == true)
	{
		scene->addEllipse(p.x(), p.y(), pen->width(), pen->width(), *pen, QBrush(Qt::SolidPattern));
		precedent = new QPoint(p);
		firstPoint = false;
	}
	else
	{
		drawLine(precedent->x(), precedent->y(), p.x(), p.y());
		precedent->setX(p.x());
		precedent->setY(p.y());	
	}
}

// Dessine une ligne à partir de deux points (deux mouvements successifs)
void LocalDrawingBoard::drawLine(int fromX, int fromY, int toX, int toY)
{
	scene->addLine(fromX, fromY, toX, toY, *pen);
}

void LocalDrawingBoard::flushScene()
{
	emit scene->clear();
	initCursor();
}
