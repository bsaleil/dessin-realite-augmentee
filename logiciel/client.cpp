#include "client.hpp"

Client::Client() : ui(new Ui::Client)
{
	ui->setupUi(this);
	
	camManager = new WebcamManager();
	camWidget = new WidgetWebcam();
	
	// Pour lister les webcams
	actionGroup = new QActionGroup(ui->menuWebcam);
	actionGroup->setExclusive(true);
	
	int nbCams = camManager->getNumberOfWebcams();
	int i=0;
	for (i=0; i<nbCams; i++)
	{
		// TODO : lister le nom des webcams plutôt que le numéro (opencv)
		QAction *action = new QAction("webcam" + QString::number(i+1), ui->menuWebcam);
		action->setCheckable(true);
		actionGroup->addAction(action);
		
		// Si c'est la première webcam, on l'active par défaut
		if (i == 0) {action->setChecked(true);}
		
		ui->menuWebcam->addAction(action);
	}
	
	connect(ui->actionD_marrer,SIGNAL(triggered()),this,SLOT(slotStart()));
}

void Client::slotStart()
{
	std::cout << "debug" << std::endl;
	// On désactive le choix des webcams
	ui->menuWebcam->setEnabled(false);
	
	// On récupère une l'image de la webcam
	QList<QAction*> listActionsWebcam = actionGroup->actions();
	int webcam = 0;
	int i=0;
	for (i=0; i<listActionsWebcam.length(); i++)
	{
		if (listActionsWebcam[i]->isChecked())
		{ webcam = listActionsWebcam[i]->text().remove("webcam").toInt(); }
	}
	
	// Récupère l'image d'initialisation
	IplImage *image;
	image = camManager->getImageInit(webcam-1);
	
	// TODO : test pour voir l'image
	cvSaveImage("image.png", image, NULL);
	
	// Appel widgetwebcam avec image
	// int ret = camWidget->calibrate(image);
	
	// Traiter retour...
	
}
