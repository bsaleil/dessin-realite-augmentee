#ifndef HPP_ETALONNAGE
#define HPP_ETALONNAGE

/*
	-- Cette classe se charge d'effectuer l'étalonnage
	-- Elle permet de faire le lien entre l'interface, et l'étalonnage de l'utilisateur.
	-- Dépendante de Qt
*/

#include <iostream>
#include <QWidget>
#include <etalonnage/webcams.hpp>

class Etalonnage
{	
	public :
		Etalonnage(int nbCams);
		QWidget* getWidget(); // Retourne le widget courant
	
	private :
		QWidget *etapeCourante;
};

#endif
