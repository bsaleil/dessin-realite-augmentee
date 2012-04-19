#include "networkdrawingboard.hpp"

NetworkDrawingBoard::NetworkDrawingBoard(QString h, int p)
{
	socket = new QTcpSocket(this);
	host = h;
	port = p;
	
	setFixedSize(645, 485);
	setSceneRect(0, 0, 640, 480);
	scene = new QGraphicsScene();
	setScene(scene);
	
	pen = new QPen();
	pen->setWidth(5);
	pen->setColor(QColor("#000000"));
	
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
	PackageManager::sendPoint(stream, point, *pen);
}		
		
void NetworkDrawingBoard::drawQPoint(QPoint p)
{
	PackageManager::sendPoint(stream, p, *pen);		
}

void NetworkDrawingBoard::drawLine(int fromX, int fromY, int toX, int toY)
{
	// TODO : envoyer l'ordre de dessiner une ligne
	qDebug() << "Dessiner ligne : " << "de (" << fromX << "," << fromY << ") à ("
		 << toX << "," << toY << ")";
}

void NetworkDrawingBoard::flushScene()
{
	// Demande de vider la scène au serveur
	PackageManager::flushScene(stream);
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
	}
	
	// Ordre de dessin
	if(command == "order")
	{
		PackageManager::order(stream, scene);
	}
}

void NetworkDrawingBoard::disconnected()
{
	qDebug() << "Déconnexion du serveur";
}



