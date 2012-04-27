#ifndef HPP_LOCALDRAWINGBOARD
#define HPP_LOCALDRAWINGBOARD

/*
	-- Cette classe implémente DrawingBoard.
	-- Elle implémente les fonctionnalités pour une utilisation locale
	-- Dépendante de Qt
*/

#include <iostream>
#include <QDebug>
#include <QColor>

#include "drawingboard.hpp"

class LocalDrawingBoard : public DrawingBoard
{	
	public :
		LocalDrawingBoard();
		void drawPoint(int x,int y);
		void drawLine(int fromX, int fromY, int toX, int toY);
		void drawQPoint(QPoint);
		void flushScene(); // Vide la scène
};

#endif
