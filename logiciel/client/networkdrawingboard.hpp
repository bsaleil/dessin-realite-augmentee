#ifndef HPP_NETWORKDRAWINGBOARD
#define HPP_NETWORKDRAWINGBOARD

/*
	-- Cette classe implémente DrawingBoard.
	-- Elle implémente les fonctionnalités pour une utilisation en réseau
	-- liée à un serveur. Le QGraphicScene local est synchronisé avec celui stocké 
	-- sur le serveur
*/

#include <iostream>
#include <QDebug>

#include "drawingboard.hpp"

class NetworkDrawingBoard : public DrawingBoard
{
	public :
		NetworkDrawingBoard();
		void drawPoint(int x,int y);
		void drawQPoint(QPoint);
		void drawLine(int fromX, int fromY, int toX, int toY);
};

#endif
