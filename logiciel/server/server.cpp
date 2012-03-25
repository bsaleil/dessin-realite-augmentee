#include "server.hpp"

Server::Server()
{
		tcpServer = new QTcpServer(this);
		tcpServer->listen(QHostAddress::Any, 34000);
		
		// Réagir à chaque connexion entrante
		connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void Server::newConnection()
{
		qDebug() << "Nouvelle connexion";
}


