#include "server.hpp"

Server::Server()
{
	tcpServer = new QTcpServer(this);
	tcpServer->listen(QHostAddress::Any, 34000);
		
	graphics = new ServerGraphics();
		
	// Réagir à chaque connexion entrante
	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
	qDebug() << "Serveur en ecoute sur le port 34000";
}

void Server::newConnection()
{
	qDebug() << "Nouvelle connexion";
	
	QTcpSocket *client = tcpServer->nextPendingConnection();
	clients << client;
	
	// On lit la socket au slot approprié
	connect(client, SIGNAL(readyRead()), this, SLOT(readData()));
	connect(client, SIGNAL(disconnected()), this, SLOT(disconnection()));

	// Récupération de la pixmap depuis la scène
	QPixmap pixmap = QPixmap::grabWidget(graphics);

	// Envoi de la scène sérialisée
	QDataStream stream(client);
	PackageManager::sendPixmap(stream, pixmap);
}

/* SLOTS */
void Server::disconnection()
{
	// Cherche qui a émit le signal
	QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
	
	// Si on ne trouve pas, on quitte
	if(client == 0)
		return;

	// On retire le client de la liste
	qDebug() << "Un client se deconnecte.";
	clients.removeOne(client);
	
	qDebug() << clients;
}

void Server::readData()
{	
	// Cherche qui a émit le signal
	QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
	
	// Si on ne trouve pas, on quitte
	if(client == 0)
		return;
	
	// Traiter le message
	QDataStream stream(client);
	QString command;
	stream >> command;
	
	QStringList list = command.split(":");

	// Commandes possibles
	if(list[0] == "order")
	{
		// On délègue le traitement du paquet
		PackageManager::order(list, clients, graphics);
	}
}
 
