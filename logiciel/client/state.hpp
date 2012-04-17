#ifndef HPP_STATE
#define HPP_STATE

/*
	-- Cette classe stocke les paramètres du dessin courant.
	-- Dépendante de Qt.
*/

#include <iostream>
#include <QColor>
#include <QDebug>

using namespace std;

class State : public QObject
{
	Q_OBJECT	
	
	public :
		State();
		// Accès aux paramètres
		QColor getColor();
		int getWidth();
		void setColor(QColor);
		//void setWidth(int);
		
		void toString();	// Affichage
	
	public slots :
		void setWidth(int);
		
	private :
		// Différents paramètres du dessin
		int width;
		QColor color;
		
		
};
#endif
