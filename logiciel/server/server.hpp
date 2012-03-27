#ifndef HPP_SERVER
#define HPP_SERVER

using namespace std;

#include <QObject>
#include <QGraphicsScene>
#include <QtDebug>
#include <QFile>
#include <QtNetwork>

#include "servergraphics.hpp"

class Server : QObject
{
	Q_OBJECT
	
	private :
		QTcpServer* tcpServer;
		QList<QTcpSocket*> clients;
			
		ServerGraphics* graphics;
		
	public :
		Server();
		
		void broadcastMessage(QObject*);		// Envoi à tous les clients l'objet en paramètre
		void messageTo(QTcpSocket* s, QObject* o);// Envoi au client spécifié	
		
	private slots :
		void newConnection();
};


#endif
