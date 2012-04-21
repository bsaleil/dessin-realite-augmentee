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
#include <QLine>
#include <QPen>

class ServerGraphics : public QGraphicsView
{
	Q_OBJECT
	
	private :
		QGraphicsScene* sceneTab;
	
	public :
		ServerGraphics();
		
		void addLine(QLine line, QPen pen);		// Ajout d'une ligne
		void addPoint(QPoint p, QPen pen);		// Ajoute un point sur la scene
		
		QGraphicsScene* getScene();

};


#endif
