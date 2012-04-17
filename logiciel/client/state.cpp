#include "state.hpp"

State::State()
{
	// On affecte les paramètres par défaut : Curseur noir de taille 5
	width = 5;
	color.setNamedColor("#000000");
}

QColor State::getColor()
{
	return color;
}

int State::getWidth()
{
	return width;
}

void State::setColor(QColor c)
{
	color = c;
}

void State::setWidth(int w)
{
	width = w;
}

void State::toString()
{
	qDebug() << "Diametre : " << QString::number(width);
	qDebug() << "Couleur : " << color.name();
}
