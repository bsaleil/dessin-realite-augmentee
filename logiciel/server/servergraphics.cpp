#include "servergraphics.hpp"

ServerGraphics::ServerGraphics()
{
	this->setFixedSize(640, 480);
	this->setSceneRect(0, 0, 640, 480);
	
	scene = new QGraphicsScene();
	this->setScene(scene);
}

void ServerGraphics::addPoint(QPoint p)
{
		
}

QGraphicsScene* ServerGraphics::getGraphicsScene()
{
	return scene;
}
