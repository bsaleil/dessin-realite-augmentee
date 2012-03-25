#ifndef HPP_SERVER
#define HPP_SERVER

using namespace std;

#include <iostream>

#include <QObject>
#include <QtDebug>
#include <QtNetwork>

class Server : QObject
{
		Q_OBJECT
		
		private :
			QTcpServer* tcpServer;
			QList<QTcpSocket*> clients;
		
		public :
			Server();
			
		private slots :
			void newConnection();
};


#endif
