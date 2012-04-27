#include "drawingboard.hpp"

void DrawingBoard::moveCursor(int x, int y)
{
	cursor->setPos(x-10,y-10);
	
	if (	x > proxyGestureWidget->x() && x < proxyGestureWidget->size().width() &&
		y > proxyGestureWidget->y() && y < proxyGestureWidget->size().height())
	{
		gestureWidget->mousePosition(x,y);
	}
}

void DrawingBoard::init()
{
	// Creation de la scene
	setFixedSize(645, 485);
	setSceneRect(0, 0, 640, 480);
	scene = new QGraphicsScene();
	setScene(scene);

	// Création du pinceau par défaut
	pen = new QPen();
		pen->setWidth(5);
		pen->setColor(QColor("#000000"));

	// Premier point
	firstPoint = true;
	
	// On initialise le curseur
	initCursor();
	// On initialise l'interface gestuelle
	initGestureUI();	
}

void DrawingBoard::showGestureWidget(bool b)
{
	proxyGestureWidget->setVisible(b);
}

void DrawingBoard::initGestureUI()
{
	// On créé l'interface
	gestureWidget = new GestureWidget(&firstPoint,pen);
	gestureWidget->setFixedWidth(this->width());
	proxyGestureWidget = scene->addWidget(gestureWidget);
	proxyGestureWidget->setOpacity(0.5);
}

void DrawingBoard::initCursor()
{
	QPixmap p("curseur.png");
	setViewport(new QGLWidget);
	
	cursor = scene->addPixmap(p);  
	
	scene->addItem(cursor);
	cursor->setPos(100,100);
}

void DrawingBoard::setFirstPoint(bool f)
{
	firstPoint = f;
}

void DrawingBoard::changeWidth(int width)
{
	pen->setWidth(width);
}

void DrawingBoard::changeColor(QColor color)
{
	pen->setColor(color);
	gestureWidget->changeButtonColor(color);
}
