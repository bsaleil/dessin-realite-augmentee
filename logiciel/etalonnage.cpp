#include "etalonnage.hpp"

Etalonnage::Etalonnage(int nbCams)
{
	Webcams *w = new Webcams();
	
	etapeCourante = w;
}

QWidget* Etalonnage::getWidget()
{
	return etapeCourante;
}
