#ifndef HPP_SERVERGRAPHICS
#define HPP_SERVERGRAPHICS

/*
 * 	Classe gérant la scene sur laquelle les clients dessinent 
 * 	La classe Server lui envoie les points à dessiner
 */

using namespace std;

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

class ServerGraphics : public QGraphicsView
{
	Q_OBJECT
	
	private :
		QGraphicsScene* sceneTab;
	
	public :
		ServerGraphics();
		void addPoint(QPoint p);		// Ajoute un point sur la scene

};


#endif
