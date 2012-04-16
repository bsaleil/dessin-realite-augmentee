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
	this->setWindowTitle("Tableau virtuel interactif");

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
	
	// Connexion à la fonction d'exportation
	connect(ui->actionExporter, SIGNAL(triggered()), this, SLOT(exportDraw()));
	
	// Fonction pour vider la scène
	connect(ui->actionFlushScene, SIGNAL(triggered()), this, SLOT(flushScene()));
	
	// Démarrage du timer
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
	timer->start(40);
}

void Client::exportDraw()
{
	// Filtre sur les extensions possibles
	QString filter;
	filter += "Images (*.png, *.jpg, *.jpeg)";
	
	// L'option DontUseNativeDialog empêche l'apparition d'un bug grisant la fenêtre
	QString fichier = QFileDialog::getSaveFileName(this, "Ouvrir un fichier", QString(), 
				filter, new QString(), QFileDialog::DontUseNativeDialog);
	
	// Exporter en pixmap d'abord
	QPixmap pixmap = QPixmap::grabWidget(drawingBoard);
	pixmap.save(fichier);
}

void Client::flushScene()
{
	drawingBoard->flushScene();
}

void Client::tick()
{		
	// Récupération de la nouvelle image
	image = camManager->getImage();
	cvFlip(image, image, 1);
	
	// Calcul de la position de l'objet suivi
	QPoint p = camWidget->getNewPosition(image,curseur);
	
	// Dessin du point
	if(curseur->active)
		drawingBoard->drawQPoint(p);
}

