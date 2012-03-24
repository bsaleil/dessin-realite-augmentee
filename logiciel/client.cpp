#include "client.hpp"

Client::Client(int w, IplImage* i, Cursor c) : ui(new Ui::Client)
{
	image = i;
	Cursor cu = c;
	curseur = &cu;

	// Construction de l'interface
	ui->setupUi(this);
	mdiArea = new QMdiArea();
	this->setCentralWidget(mdiArea);
	
	// Classes utilisées
	// TODO instancier selon l'état réseau ou local
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

Client::Client(int w, IplImage* i, Cursor c, QString h, int p)
{
	Client(w,i,c);
	qDebug() << "Host : " << h << "\n" << "Port : " << p;
}

void Client::tick()
{
	// Récupération de la nouvelle image
	image = camManager->getImage();
	// Calcul de la position de l'objet suivi
	QPoint p = camWidget->getNewPosition(image,curseur);
	// Dessin du point
	drawingBoard->drawQPoint(p);
}
