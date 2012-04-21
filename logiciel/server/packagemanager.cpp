#include "packagemanager.hpp"

void PackageManager::sendPixmap(QDataStream &stream, QPixmap pixmap)
{
	stream << QString("item");
	stream << QString("qpixmap");
	stream << pixmap;
}

void PackageManager::sendLine(QDataStream &stream, QLine line, QPen pen)
{
	QString command("order");
	QString type("qline");
	
	stream << command << type;
	stream << line;
	stream << pen;
}

void PackageManager::sendPoint(QDataStream &stream, QPoint point, QPen pen)
{
	QString command("order");
	QString type("qpoint");
	
	stream << command << type;
	stream << point;
	stream << pen;
}

void PackageManager::order(QDataStream &stream, QList<QTcpSocket *> &clients, ServerGraphics* graphics)
{
	// Type de l'ordre
	QString type;
	stream >> type;
	
	if(type == "qpoint")
	{
		QPoint point;	
		stream >> point;
		
		// Récupération des options
		QPen pen;
		stream >> pen;
			
		// Ajout du point sur la scène
		graphics->addPoint(point, pen);
			
		// Envoi à tous les clients du point à dessiner
		broadcastPoint(clients, point, pen);
	}
	
	if(type == "qline")
	{
		QLine line;	
		stream >> line;
		
		// Récupération des options
		QPen pen;
		stream >> pen;
			
		// Ajout de la ligne la scène
		graphics->addLine(line, pen);
			
		// Envoi à tous les clients de la ligne à dessiner
		broadcastLine(clients, line, pen);	
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

void PackageManager::broadcastPoint(QList<QTcpSocket *> &clients, QPoint point, QPen pen)
{
	for(int i=0; i <clients.size(); ++i)
	{
		QDataStream stream(clients.at(i));
		
		// Envoi du point à un client
		sendPoint(stream, point, pen);
	}
}

void PackageManager::broadcastLine(QList<QTcpSocket *> &clients, QLine line, QPen pen)
{
	for(int i=0; i <clients.size(); ++i)
	{
		QDataStream stream(clients.at(i));
	
		sendLine(stream, line, pen);
	}
}


