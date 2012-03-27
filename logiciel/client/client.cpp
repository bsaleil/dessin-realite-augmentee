#include "client.hpp"

Client::Client(int w, IplImage* i, Cursor c) : ui(new Ui::Client)
{
	// Utilisation de drawingboard en local
	drawingBoard = new LocalDrawingBoard();
	// Initialisation commune
	init(w,i,c);
}

Client::Client(int w, IplImage* i, Cursor c, QString h, int p) : ui(new Ui::Client)
{	
	// Utilisation de drawingboard en réseau
	drawingBoard = new NetworkDrawingBoard(h, p);
	// Initialisation commune
	init(w,i,c);
}

void Client::init(int w, IplImage *i, Cursor c)
{
	image = i;
	curseur = new Cursor;
	*curseur = c; 

	// Construction de l'interface
	ui->setupUi(this);
	mdiArea = new QMdiArea();
	this->setCentralWidget(mdiArea);

	// Classes utilisées
	camManager = new WebcamManager();
	camWidget = new WidgetWebcam();
	
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
