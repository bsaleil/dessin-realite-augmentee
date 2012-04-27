#ifndef HPP_GESTUREBUTTON
#define HPP_GESTUREBUTTON

/*
	Cette classe sert à adapter les QPushButton de Qt a des boutons pour notre interface gestuelle.
*/

#include <iostream>
#include <QPushButton>
#include <QPoint>
#include <QDebug>

using namespace std;

class GestureButton : public QPushButton
{
	private :
		QString id;

	public :
		// Construire avec id
		GestureButton(QString i) : QPushButton()
		{
			id = i;
		}
	
		// Construire avec id et texte à afficher
		GestureButton(QString i,QString text) : QPushButton(text)
		{
			id = i;
		}
		
		// Retourne l'id du bouton
		QString click()
		{
			return id;
		}
};

#endif
