#include "localdrawingboard.hpp"

LocalDrawingBoard::LocalDrawingBoard()
{
	setFixedSize(640,480);
	scene = new QGraphicsScene();
	setScene(scene);
	qDebug() << "drawing";
}

void LocalDrawingBoard::drawPoint(int x,int y)
{
	scene->addEllipse(x,y,5,5,QPen(),QBrush(Qt::SolidPattern));
}
void LocalDrawingBoard::drawLine(int fromX, int fromY, int toX, int toY)
{
}
