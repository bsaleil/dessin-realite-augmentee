#ifndef HPP_CLIENTPACKAGEMANAGER
#define HPP_CLIENTPACKAGEMANAGER

/*
 * 	Classe gérant les paquets envoyés par le serveur, le message est 
 * 	identifié en amont, et le PackageManager agit suivant la commande
 */

using namespace std;

#include <QDataStream>
#include <QGraphicsScene>
#include <QPoint>

class PackageManager
{
	public :
		// Envoi d'un ordre de dessin au serveur
		static void sendPoint(QDataStream &stream, QPoint p);

		// Envoie une demande de vidage de scène
		static void flushScene(QDataStream &stream);
	
		// Reçoit un objet et le traite en conséquence
		static void item(QDataStream &stream, QGraphicsScene* scene);
		
		// Reçoit un ordre du serveur
		static void order(QDataStream &stream, QGraphicsScene* scene);
};

#endif
