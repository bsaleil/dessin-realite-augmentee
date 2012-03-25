#include "client.hpp"

Client::Client(int w, IplImage* i, Cursor c) : ui(new Ui::Client)
{	
	image = i;
	curseur = new Cursor;
	*curseur = c; 

	// Construction de l'interface
	ui->setupUi(this);
	mdiArea = new QMdiArea();
	this->setCentralWidget(mdiArea);
	
	// TODO : Nettoyage des classes client et webcams avec renommage
	// Classes utilisées
	drawingBoard = new LocalDrawingBoard();
	camManager = new WebcamManager();
	camWidget = new WidgetWebcam();
	
	// TODO possibilité de stopper ?
	// Demarrage de la webcam
	camManager->setWebcam(w);
	camManager->runWebcam();
	
	// Lancement du widget d'affichage de la webcam
	mdiArea->addSubWindow(drawingBoard);
	
	// Lancement du widget de dessin
	mdiArea->addSubWindow(camWidget);
	
	// Démarrage du timer
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
	timer->start(40);
}

Client::Client(int w, IplImage* i, Cursor c, QString h, int p) : ui(new Ui::Client)
{
	Client(w, i, c);
	
	// Instanciation du drawingBoard en réseau
	drawingBoard = new NetworkDrawingBoard(h, p);
	
	qDebug() << "Host : " << h << "\n" << "Port : " << p;
}

void Client::tick()
{		
	// Récupération de la nouvelle image
	image = camManager->getImage();
	cvFlip(image, image,1);
	// Calcul de la position de l'objet suivi
	QPoint p = camWidget->getNewPosition(image,curseur);
	// Dessin du point
	drawingBoard->drawQPoint(p);
}

