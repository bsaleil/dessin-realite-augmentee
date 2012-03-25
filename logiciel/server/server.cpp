#include "server.hpp"

Server::Server()
{
	tcpServer = new QTcpServer(this);
	tcpServer->listen(QHostAddress::Any, 34000);
		
	graphics = new ServerGraphics();
		
	// Réagir à chaque connexion entrante
	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void Server::newConnection()
{
	qDebug() << "Nouvelle connexion";
	
	QTcpSocket *client = tcpServer->nextPendingConnection();
	clients << client;
	
	qDebug() << graphics->getGraphicsScene();
	messageTo(client, graphics->getGraphicsScene());
}

void Server::broadcastMessage(QObject* o)
{
	
}

void Server::messageTo(QTcpSocket* s, QObject* o)
{
	//s->write(o);
}

