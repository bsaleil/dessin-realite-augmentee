#include "servergraphics.hpp"

ServerGraphics::ServerGraphics()
{
	setFixedSize(640, 480);
	setSceneRect(0, 0, 640, 480);
	
	scene = new QGraphicsScene();
	setScene(scene);
}

void ServerGraphics::addPoint(QPoint p)
{
	
}

QGraphicsScene* ServerGraphics::getGraphicsScene()
{
	return scene;
}
