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

void PackageManager::order(QStringList list, QList<QTcpSocket *> &clients, ServerGraphics* graphics)
{	
	if(list[1] == "point")
	{
		// Ajout du point sur la scène
		QPen pen(list[4]); pen.setWidth(list[5].toInt());
		graphics->addPoint(QPoint(list[2].toInt(),list[3].toInt()), pen);
			
		// Envoi à tous les clients du point à dessiner
		broadcast(clients, list);
	}
	
	else if(list[1] == "line")
	{	
		// Ajout de la ligne la scène
		QPen pen(list[6]); pen.setWidth(list[7].toInt());
		graphics->addLine(QLine(list[2].toInt(),list[3].toInt(),
					list[4].toInt(),list[5].toInt()), pen);
			
		// Envoi à tous les clients de la ligne à dessiner
		broadcast(clients, list);	
	}
	
	else if(list[1] == "flush")
	{
		// Déclenchement du signal pour vider la scène
		emit graphics->getScene()->clear();
		
		// Signale aux clients de nettoyer la scène
		broadcast(clients, list);
	}
}

void PackageManager::broadcast(QList<QTcpSocket *> &clients, QStringList list)
{
	for(int i=0; i <clients.size(); ++i)
	{
		QDataStream stream(clients.at(i));
		
		stream << list.join(":");
	}
}
