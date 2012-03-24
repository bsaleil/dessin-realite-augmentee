#include "localdrawingboard.hpp"

LocalDrawingBoard::LocalDrawingBoard()
{
	setFixedSize(640,480);
	setSceneRect(0,0,640,480);
	scene = new QGraphicsScene();
	setScene(scene);
}

void LocalDrawingBoard::drawPoint(int x, int y)
{
	scene->addEllipse(x, y, 5, 5, QPen(), QBrush(Qt::SolidPattern));
}

// Dessine un point sur le tableau
void LocalDrawingBoard::drawQPoint(QPoint p)
{
	scene->addEllipse(p.x(), p.y(), 5, 5, QPen(), QBrush(Qt::SolidPattern));
}

// Dessine une ligne à partir de deux points (deux mouvements successifs)
void LocalDrawingBoard::drawLine(int fromX, int fromY, int toX, int toY)
{
	scene->addLine(fromX, fromY, toX, toY, QPen());
}
