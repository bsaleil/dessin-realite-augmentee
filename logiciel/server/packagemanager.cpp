#include "packagemanager.hpp"

void PackageManager::sendPixmap(QDataStream &stream, QPixmap pixmap)
{
	stream << QString("item");
	stream << QString("qpixmap");
	stream << pixmap;
}

void PackageManager::sendLine(QDataStream &stream, QLine line, QPen pen)
{
	// "order:line:x1:y1:x2:y3:color:size"
	string packet = "order:line:";
	packet += QString::number(line.x1()).toStdString() + ":";
	packet += QString::number(line.y1()).toStdString() + ":";
	packet += QString::number(line.x2()).toStdString() + ":";
	packet += QString::number(line.y2()).toStdString() + ":";
	packet += pen.color().name().toStdString() + ":";
	packet += QString::number(pen.width()).toStdString();
	
	stream.writeRawData(packet.c_str(), strlen(packet.c_str()));
}

void PackageManager::sendPoint(QDataStream &stream, QPoint point, QPen pen)
{	
	// "order:point:x:y:color:size"
	string packet = "order:point:";
	packet += QString::number(point.x()).toStdString() + ":";
	packet += QString::number(point.y()).toStdString() + ":";
	packet += pen.color().name().toStdString() + ":";
	packet += QString::number(pen.width()).toStdString();
	
	stream.writeRawData(packet.c_str(), strlen(packet.c_str()));
}

void PackageManager::order(QDataStream &stream, QList<QTcpSocket *> &clients, ServerGraphics* graphics)
{
	// Type de l'ordre
	QString type;
	stream >> type;
	
	if(type == "point")
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


