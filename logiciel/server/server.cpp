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

	QGraphicsScene *scene = graphics->getGraphicsScene();
	QGraphicsView* view = new QGraphicsView(scene);
	
	QPixmap pixmap = QPixmap::grabWidget(view);

	// Envoi de la scène sérialisée
	QDataStream stream(client);
	stream << QString("item");
	stream << QString("qpixmap");
	stream << pixmap;
}

void Server::broadcastMessage(QObject* o)
{
	// TODO : foreach client, send
}

void Server::messageTo(QTcpSocket* s, QObject* o)
{
	
}

