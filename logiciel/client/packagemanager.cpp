#include "packagemanager.hpp"

QString PackageManager::sendPoint(QDataStream &stream, QPoint point, QPen pen, QString old)
{	
	// "order:point:x:y:color:size"
	QString packet = "order:point:";
	packet += QString::number(point.x()) + ":";
	packet += QString::number(point.y()) + ":";
	packet += pen.color().name() + ":";
	packet += QString::number(pen.width());
	
	// Si le paquet est différent du précédent, on envoit
	if (packet != old)
		stream << packet;
	return packet;
	
}

QString PackageManager::sendLine(QDataStream &stream, QLine line, QPen pen, QString old)
{
	// "order:line:x1:y1:x2:y3:color:size"
	QString packet = "order:line:";
	packet += QString::number(line.x1()) + ":";
	packet += QString::number(line.y1()) + ":";
	packet += QString::number(line.x2()) + ":";
	packet += QString::number(line.y2()) + ":";
	packet += pen.color().name() += ":";
	packet += QString::number(pen.width());
	
	if (packet != old)
		stream << packet;
	return packet;
}

void PackageManager::flushScene(QDataStream &stream)
{
	// Envoie d'une demande de vidage de scène
	QString command("order:flush");
	stream << command;
}

void PackageManager::item(QDataStream &stream, QGraphicsScene* scene)
{
	// Type de l'objet reçu
	QString type;
	stream >> type;
	
	// Demande de synchronisation de tableau
	if(type == "qpixmap")
	{
		QPixmap pixmap;
			
		// TODO : corriger le warning 
		stream >> pixmap;
			
		// On mets le pixmap dans la scène locale
		scene->addPixmap(pixmap);
	}
}

void PackageManager::order(QStringList list, QGraphicsScene* scene, DrawingBoard* view)
{
	if(list[1] == "point")
	{
		QPen pen(list[4]); pen.setWidth(list[5].toInt());
		// Dessiner sur le tableau local
		scene->addEllipse(list[2].toInt(), list[3].toInt(), list[5].toInt(), list[5].toInt(), pen, QBrush(Qt::SolidPattern));
	}
	
	else if(list[1] == "line")
	{
		QPen pen(list[6]); pen.setWidth(list[7].toInt());		
		// Dessin d'une ligne en local
		scene->addLine(QLine(list[2].toInt(),list[3].toInt(),
				list[4].toInt(),list[5].toInt()), pen);
	}
	
	else if(list[1] == "flush")
	{
		emit scene->clear();
		
		// Refaire le curseur et l'interface gestuelle
		view->initCursor();
		view->initGestureUI();
	}
}

