#ifndef HPP_PACKAGEMANAGER
#define HPP_PACKAGEMANAGER

using namespace std;

/*
*	Classe traitant les paquets reçus par le serveur
*
*/

#include <QDataStream>
#include <QPixmap>
#include <QString>

class PackageManager
{
	public :
		// Envoi au client un objet pixmap
		static void sendPixmap(QDataStream &stream, QPixmap pixmap);
		
		// Envoi d'un point
		static void sendPoint(QDataStream &stream, QPoint point);


};


#endif
