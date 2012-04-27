#include "packagemanager.hpp"

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

void PackageManager::flushScene(QDataStream &stream)
{
	// Envoie d'une demande de vidage de scène
	QString command("order");
	QString type("flush");
	
	stream << command;
	stream << type;
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

void PackageManager::order(QDataStream &stream, QGraphicsScene* scene, DrawingBoard* view)
{
	// Type de l'objet reçu
	QString type;
	stream >> type;

	if(type == "point")
	{
		QPoint p;
		stream >> p;
		
		// Récupération des options
		QPen pen;
		stream >> pen;
	
		// Dessiner sur le tableau local
		scene->addEllipse(p.x(), p.y(), pen.width(), pen.width(), pen, QBrush(Qt::SolidPattern));
	}
	
	if(type == "line")
	{
		QLine line;
		stream >> line;
	
		QPen pen;
		stream >> pen;
		
		// Dessin d'une ligne en local
		scene->addLine(line, pen);
	}
	
	if(type == "flush")
	{
		emit scene->clear();
		
		// Refaire le curseur et l'interface gestuelle
		view->initCursor();
		view->initGestureUI();
	}
}

