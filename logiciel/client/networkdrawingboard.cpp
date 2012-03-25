#include "networkdrawingboard.hpp"

NetworkDrawingBoard::NetworkDrawingBoard(QString h, int p)
{
	socket = new QTcpSocket(this);
	host = h;
	port = p;
	
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
{}		
		
void NetworkDrawingBoard::drawQPoint(QPoint)
{}

void NetworkDrawingBoard::drawLine(int fromX, int fromY, int toX, int toY)
{}

void NetworkDrawingBoard::connectionActive()
{
	qDebug() << "Connexion active";
}

void NetworkDrawingBoard::dataIncoming()
{
	qDebug() << "Réception de données";
}

void NetworkDrawingBoard::disconnected()
{
	qDebug() << "Déconnexion du serveur";
}


