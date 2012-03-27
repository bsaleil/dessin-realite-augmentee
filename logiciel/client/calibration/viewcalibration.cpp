#include "viewcalibration.hpp"

ViewCalibration::ViewCalibration(QPoint *A, QPoint *B) : QGraphicsView()
{
	pointA = A;
	pointB = B;
	nbClicsEffectues = 0;
	
	scene = new QGraphicsScene();
	setScene(scene);
}

void ViewCalibration::mousePressEvent(QMouseEvent* ev)
{
	if (ev->x() >= 0 && ev->x() <= largeurImage && ev->y() >= 0 && ev->y() <= hauteurImage)
	{
		// Si premier clic
		if (nbClicsEffectues == 0)
		{
			*pointA = ev->pos();
			nbClicsEffectues++;
			scene->addEllipse(ev->x()-2, ev->y()-2, 4, 4, QPen(QColor("red")), QBrush(Qt::SolidPattern));
		}
		// Sinon si deuxieme clic
		else if (nbClicsEffectues == 1)
		{	
			*pointB = ev->pos();
			nbClicsEffectues++;
			scene->addEllipse(ev->x()-2, ev->y()-2, 4, 4, QPen(QColor("red")), QBrush(Qt::SolidPattern));
			
			scene->addEllipse(pointA->x()-2, pointB->y()-2, 4, 4, QPen(QColor("red")), QBrush(Qt::SolidPattern));
			scene->addEllipse(pointB->x()-2, pointA->y()-2, 4, 4, QPen(QColor("red")), QBrush(Qt::SolidPattern));
			
			// Lignes
			scene->addLine(pointA->x(),pointA->y(),pointA->x(),pointB->y(),QPen(QColor("red")));
			scene->addLine(pointA->x(),pointA->y(),pointB->x(),pointA->y(),QPen(QColor("red")));
			scene->addLine(pointB->x(),pointB->y(),pointA->x(),pointB->y(),QPen(QColor("red")));
			scene->addLine(pointB->x(),pointB->y(),pointB->x(),pointA->y(),QPen(QColor("red")));
		}
	}
}

void ViewCalibration::addPixmap(QPixmap p)
{
	largeurImage = p.width();
	hauteurImage = p.height();
	
	setFixedSize(largeurImage,hauteurImage);
	
	scene->addPixmap(p);
}
