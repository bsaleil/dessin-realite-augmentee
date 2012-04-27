#include "localdrawingboard.hpp"

LocalDrawingBoard::LocalDrawingBoard()
{		
	init();
	
	QPoint p;
	QDataStream s; s << p;
	
	QString sb = "10:40:10:#000000";
	QDataStream ssb; ssb << sb;
	
	qDebug() << sizeof(s);
	qDebug() << sizeof(sb);
	
}

void LocalDrawingBoard::drawPoint(int x, int y)
{	
	bool canDraw = !(x > gestureWidget->pos().x() && x < gestureWidget->size().width() &&
			y > gestureWidget->pos().y() && y < gestureWidget->size().height());
	
	if (canDraw)
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
}

// Dessine un point sur le tableau
void LocalDrawingBoard::drawQPoint(QPoint p)
{
	drawPoint(p.x(),p.y());
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
	initGestureUI();
}
