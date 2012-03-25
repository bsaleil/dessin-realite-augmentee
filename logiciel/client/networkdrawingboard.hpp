#ifndef HPP_NETWORKDRAWINGBOARD
#define HPP_NETWORKDRAWINGBOARD

/*
	-- Cette classe implémente DrawingBoard.
	-- Elle implémente les fonctionnalités pour une utilisation en réseau
	-- liée à un serveur. Le QGraphicScene local est synchronisé avec celui stocké 
	-- sur le serveur
*/

#include <QDebug>
#include <QTcpSocket>

#include "drawingboard.hpp"

class NetworkDrawingBoard : public DrawingBoard
{
	Q_OBJECT
	
	public :
		NetworkDrawingBoard(QString, int);	// Hôte et port 
		void drawPoint(int x,int y);
		void drawQPoint(QPoint);
		void drawLine(int fromX, int fromY, int toX, int toY);
	
	private :
		QTcpSocket* socket;			// Communication avec le serveur
		QString host;
		int port;
	
	private slots :
		void connectionActive();		// Connexion active
		void dataIncoming();			// Réception de données
		void disconnected();			// Déconnexion du serveur
};

#endif


