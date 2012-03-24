#ifndef HPP_LABELETALONNAGE
#define HPP_LABELETALONNAGE

/*
	-- Label d'image pour l'etalonnage
*/

#include <iostream>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>
#include <QDialog>
#include <QTime>
#include <QMoveEvent>
#include <QGraphicsView>

class ViewCalibration : public QGraphicsView
{	
	Q_OBJECT

	public :
		ViewCalibration(QPoint* pointA, QPoint* pointB);
		void addPixmap(QPixmap p);
	protected :
		void mousePressEvent(QMouseEvent* ev);
		
	private :
		QGraphicsScene *scene;
	
		int nbClicsEffectues;
	
		int largeurImage;
		int hauteurImage;
		
		QPoint *pointA;
		QPoint *pointB;
		
		QPoint *pointC;
		QPoint *pointD;
};

#endif
