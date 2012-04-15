#include "packagemanager.hpp"

void PackageManager::sendPixmap(QDataStream &stream, QPixmap pixmap)
{
	stream << QString("item");
	stream << QString("qpixmap");
	stream << pixmap;
}

void PackageManager::sendPoint(QDataStream &stream, QPoint point)
{
	QString command("order");
	QString type("qpoint");
	
	stream << command << type;
	stream << point;
}

void PackageManager::order(QDataStream &stream, QList<QTcpSocket *> &clients, ServerGraphics* graphics)
{
	// Type de l'ordre
	QString type;
	stream >> type;
	
	if(type == "qpoint")
	{
		QPoint p;	
		stream >> p;
			
		// Ajout du point sur la scène
		graphics->addPoint(p);
			
		// Envoi à tous les clients du point à dessiner
		broadcastPoint(clients, p);
	}
	
	if(type == "flush")
	{
		// Déclenchement du signal pour vider la scène
		emit graphics->getScene()->clear();
		
		// Signale aux clients de nettoyer la scène
		stream << QString("order");
		stream << QString("flush");
	}
}

void PackageManager::broadcastPoint(QList<QTcpSocket *> &clients, QPoint point)
{
	for(int i=0; i <clients.size(); ++i)
	{
		QDataStream stream(clients.at(i));
		
		// Envoi du point à un client
		sendPoint(stream, point);
	}
}

