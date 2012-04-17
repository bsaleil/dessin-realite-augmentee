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
}

void LocalDrawingBoard::drawPoint(int x, int y)
{
	scene->addEllipse(x, y, pen->width(), pen->width(), *pen, QBrush(Qt::SolidPattern));
}

// Dessine un point sur le tableau
void LocalDrawingBoard::drawQPoint(QPoint p)
{
	scene->addEllipse(p.x(), p.y(), pen->width(), pen->width(), *pen, QBrush(Qt::SolidPattern));
}

// Dessine une ligne à partir de deux points (deux mouvements successifs)
void LocalDrawingBoard::drawLine(int fromX, int fromY, int toX, int toY)
{
	scene->addLine(fromX, fromY, toX, toY, *pen);
}

void LocalDrawingBoard::flushScene()
{
	emit scene->clear();
}