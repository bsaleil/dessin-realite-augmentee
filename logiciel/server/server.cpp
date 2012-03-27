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
	
	// On lit la socket au slot approprié
	connect(client, SIGNAL(readyRead()), this, SLOT(readData()));

	QPixmap pixmap = QPixmap::grabWidget(graphics);

	// Envoi de la scène sérialisée
	QDataStream stream(client);
	stream << QString("item");
	stream << QString("qpixmap");
	stream << pixmap;
}

void Server::sendPoint(QPoint point)
{
	for(int i=0; i <clients.size(); ++i)
	{
		QDataStream stream(clients.at(i));
		QString command("order");
		QString type("qpoint");
		stream << command << type;
		stream << point;
	}
}

void Server::messageTo(QTcpSocket* s, QObject* o)
{
	
}

void Server::readData()
{	
	// Cherche qui a émit le signal
	QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
	
	if(client == 0)
		return;
	
	// Traiter le message
	QDataStream stream(client);
	
	QString command;
	QString type;
	
	stream >> command;
	stream >> type;
	
	// TODO : factoriser
	if(command == "order")
	{
		if(type == "qpoint")
		{
			QPoint p;	
			stream >> p;
			qDebug() << command << "    " << type << "Point " << p;
			
			graphics->addPoint(p);
			
			// Envoi à tous les clients du point à dessiner
			sendPoint(p);
		}
	}
}
 
