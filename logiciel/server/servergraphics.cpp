#include "servergraphics.hpp"

ServerGraphics::ServerGraphics()
{
	qDebug() << "tableau du serveur";
	scene = new QGraphicsScene(this);
	
	this->setScene(scene);
}

void ServerGraphics::addPoint(QPoint p)
{
		
}

QGraphicsScene* ServerGraphics::getGraphicsScene()
{
	return scene;
}
