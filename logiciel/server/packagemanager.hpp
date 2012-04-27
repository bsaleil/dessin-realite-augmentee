#ifndef HPP_PACKAGEMANAGER
#define HPP_PACKAGEMANAGER

using namespace std;

/*
*	Classe traitant les paquets reçus par le serveur
*
*/

#include <iostream>
#include <string>
#include <QDataStream>
#include <QLine>
#include <QList>
#include <QPixmap>
#include <QPen>
#include <QString>
#include <QtNetwork>
#include "servergraphics.hpp"

class PackageManager
{
	public :
		// Broadcast d'un paquet déjà découpé
		static void broadcast(QList<QTcpSocket *> &clients, QStringList list);
		
		// Envoi d'un point à un seul client
		static void sendPoint(QDataStream &stream, QPoint point, QPen pen);
		
		// Envoi d'une ligne à un seul client
		static void sendLine(QDataStream &stream, QLine line, QPen pen);

		// Envoi au client un objet pixmap
		static void sendPixmap(QDataStream &stream, QPixmap pixmap);
		
		// Traite une commande de type ordre, en agissant sur la scène
		static void order(QStringList list, QList<QTcpSocket *> &clients, ServerGraphics* graphics);
};


#endif
