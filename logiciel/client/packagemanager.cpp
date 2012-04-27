#include "packagemanager.hpp"

void PackageManager::sendPoint(QDataStream &stream, QPoint point, QPen pen)
{
	// Envoie l'ordre de dessin au serveur
	QString command("order");
	QString type("qpoint");
	
	stream << command;
	stream << type;
	stream << point;
	stream << pen;
}

void PackageManager::sendLine(QDataStream &stream, QLine line, QPen pen)
{
	// Envoie l'ordre de dessin au serveur
	QString command("order");
	QString type("qline");
	
	stream << command;
	stream << type;
	stream << line;
	stream << pen;
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

	if(type == "qpoint")
	{
		QPoint p;
		stream >> p;
		
		// Récupération des options
		QPen pen;
		stream >> pen;
	
		// Dessiner sur le tableau local
		scene->addEllipse(p.x(), p.y(), pen.width(), pen.width(), pen, QBrush(Qt::SolidPattern));
	}
	
	if(type == "qline")
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

