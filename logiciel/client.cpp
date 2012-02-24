#include "client.hpp"

Client::Client() : ui(new Ui::Client)
{
	ui->setupUi(this);
	mdiArea = new QMdiArea();
	this->setCentralWidget(mdiArea);
	
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
	
	// On récupère la webcam sélectionnée
	QList<QAction*> listActionsWebcam = actionGroup->actions();
	webcamActive = 0;
	int i=0;
	for (i=0; i<listActionsWebcam.length(); i++)
	{
		if (listActionsWebcam[i]->isChecked())
		{ webcamActive = listActionsWebcam[i]->text().remove("webcam").toInt(); }
	}
	
	// La numérotation commence à 0
	webcamActive--;
	calibration();
}


void Client::calibration()
{
	// Temps avant de prendre une image pour l'étalonnage
	calibrationCounter = 5;
	
	// Affichage du compteur
	labelCounter = new QLabel("Etalonnage dans : <br/><span style=\"font-size:100px;\">"+QString::number(calibrationCounter)+"</span>");

	labelCounter->setAlignment(Qt::AlignCenter); labelCounter->setFixedSize(640, 480);
	mdiArea->addSubWindow(labelCounter);
	labelCounter->show();
	
	// Démarrage du timer
	timerCounter = new QTimer(this);
	connect(timerCounter, SIGNAL(timeout()), this, SLOT(slotCounterChange()));
	timerCounter->start(1000);
}


void Client::slotCounterChange()
{
	if (calibrationCounter > 0)
	{
		labelCounter->setText("Etalonnage dans : <br/><span style=\"font-size:100px;\">"+QString::number(calibrationCounter)+"</span>");
		labelCounter->update();
		calibrationCounter--;
	}
	else
	{
		timerCounter->stop();
			
		// Récupère l'image d'initialisation
		IplImage *image;
		image = camManager->getImageInit(webcamActive);

		// Appel widgetwebcam avec image
		// TODO : retour.
		camWidget->calibrate(image);
	
		// Traiter retour...
	}
}

