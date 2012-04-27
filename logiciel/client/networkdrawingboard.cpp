#include "networkdrawingboard.hpp"

NetworkDrawingBoard::NetworkDrawingBoard(QString h, int p)
{
	socket = new QTcpSocket(this);
	host = h;
	port = p;
	
	init();
	
	// Connexion au serveur
	socket->abort();
	socket->connectToHost(host, port);
	
	// Signal levé lorsque la connexion est active
	connect(socket, SIGNAL(connected()), this, SLOT(connectionActive()));
	
	// Signal levé lors de la réception de données
	connect(socket, SIGNAL(readyRead()), this, SLOT(dataIncoming()));
	
	// Déconnexion
	connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}		

void NetworkDrawingBoard::drawPoint(int x, int y)
{	
	
	// Délègue au package manager
	QPoint point(x, y);
	if(firstPoint == true)
	{
		PackageManager::sendPoint(stream, point, *pen);		
		firstPoint = false;
		precedent = new QPoint(point);
	}
	else
	{
		// Envoi d'une ligne à partir du point précédent et courant
		QLine line(*precedent, point);
		PackageManager::sendLine(stream, line, *pen);
		precedent->setX(point.x());
		precedent->setY(point.y());
	}
}		
		
void NetworkDrawingBoard::drawQPoint(QPoint point)
{	
	if(firstPoint == true)
	{
		PackageManager::sendPoint(stream, point, *pen);		
		firstPoint = false;
		precedent = new QPoint(point);
	}
	else
	{
		QLine line(*precedent, point);
		PackageManager::sendLine(stream, line, *pen);
		precedent->setX(point.x());
		precedent->setY(point.y());
	}
}

void NetworkDrawingBoard::drawLine(int fromX, int fromY, int toX, int toY)
{
	QPoint pointFrom(fromX, fromY);
	QPoint pointTo(toX, toY);
	QLine line(pointFrom, pointTo);
	
	PackageManager::sendLine(stream, line, *pen);
}

void NetworkDrawingBoard::flushScene()
{
	// Demande de vider la scène au serveur
	PackageManager::flushScene(stream);
	initGestureUI();
}

void NetworkDrawingBoard::connectionActive()
{
	qDebug() << "Connexion active";
	
	// Lie la socket au flux
	stream.setDevice(socket);
}

void NetworkDrawingBoard::dataIncoming()
{
	// On regarde la commande
	QString command;
	stream >> command;
	
	// Réception d'un objet
	if(command == "item")
	{	
		PackageManager::item(stream, scene);
		initCursor();
		initGestureUI();
	}
		
	// Ordre de dessin
	if(command == "order")
	{
		PackageManager::order(stream, scene, this);
	}
}

void NetworkDrawingBoard::disconnected()
{
	qDebug() << "Déconnexion du serveur";
}



