#include "networkdrawingboard.hpp"

NetworkDrawingBoard::NetworkDrawingBoard(QString h, int p)
{
	socket = new QTcpSocket(this);
	host = h;
	port = p;
	
	setFixedSize(640, 480);
	setSceneRect(0, 0, 640, 480);
	scene = new QGraphicsScene();
	setScene(scene);
	
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
	// Envoie l'ordre de dessin au serveur
}		
		
void NetworkDrawingBoard::drawQPoint(QPoint p)
{	
	// Envoie l'ordre de dessin au serveur
	QString command("order");
	QString type("qpoint");
	
	stream << command;
	stream << type;
	stream << p;
}

void NetworkDrawingBoard::drawLine(int fromX, int fromY, int toX, int toY)
{}

void NetworkDrawingBoard::connectionActive()
{
	qDebug() << "Connexion active";
	
	// Lie la socket au flux
	stream.setDevice(socket);
}

void NetworkDrawingBoard::dataIncoming()
{
	qDebug() << "Réception de données";
	
	// On regarde le message
	QString command;
	stream >> command;
	
	if(command == "item")
	{	
		QString type;
		stream >> type;
		if(type == "qpixmap")
		{
			// Synchronisation de la scène avec le serveur
			QPixmap pixmap;
			
			stream >> pixmap;
			pixmap.save("image.png");
			
			// On mets le pixmap dans la scène locale
			scene->addPixmap(pixmap);
		}
	}
	if(command == "order")
	{
		// TODO : dessiner au point indiqué par le serveur
		QString type;
		stream >> type;
		
		if(type == "qpoint")
		{
			QPoint p;
			stream >> p;
			
			// Dessiner sur le tableau local
			scene->addEllipse(p.x(), p.y(), 5, 5, QPen(), QBrush(Qt::SolidPattern));
		}
	}
}

void NetworkDrawingBoard::disconnected()
{
	qDebug() << "Déconnexion du serveur";
}



