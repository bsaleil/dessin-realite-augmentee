#include "networkdrawingboard.hpp"

NetworkDrawingBoard::NetworkDrawingBoard(QString h, int p)
{
	host = h;
	port = p;
}		
		
void NetworkDrawingBoard::drawPoint(int x, int y)
{}		
		
void NetworkDrawingBoard::drawQPoint(QPoint)
{}

void NetworkDrawingBoard::drawLine(int fromX, int fromY, int toX, int toY)
{}
