#ifndef HPP_PACKAGEMANAGER
#define HPP_PACKAGEMANAGER

using namespace std;

/*
*	Classe traitant les paquets reçus par le serveur
*
*/

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
		// Envoi d'un point à tous les clients
		static void broadcastPoint(QList<QTcpSocket *> &clients, QPoint point, QPen pen);

		// Envoi d'une ligne à tous les clients
		static void broadcastLine(QList<QTcpSocket *> &clients, QLine line, QPen pen);
		
		// Envoi d'un point à un seul client
		static void sendPoint(QDataStream &stream, QPoint point, QPen pen);
		
		// Envoi d'une ligne à un seul client
		static void sendLine(QDataStream &stream, QLine line, QPen pen);

		// Envoi au client un objet pixmap
		static void sendPixmap(QDataStream &stream, QPixmap pixmap);
		
		// Traite une commande de type ordre, en agissant sur la scène
		static void order(QDataStream &stream, QList<QTcpSocket *> &clients, ServerGraphics* graphics);
};


#endif
