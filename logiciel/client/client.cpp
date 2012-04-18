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

	// Classes utilisées
	camManager = new WebcamManager();
	camWidget = new WidgetWebcam();
	
	this->setWindowTitle("Tableau virtuel interactif");
	
	// Demarrage de la webcam
	camManager->setWebcam(w);
	camManager->runWebcam();
	
	// Connexion à la fonction d'exportation
	connect(ui->actionExporter, SIGNAL(triggered()), this, SLOT(exportDraw()));
	
	// Fonction pour vider la scène
	connect(ui->actionFlushScene, SIGNAL(triggered()), this, SLOT(flushScene()));
	
	// Connexion au plein écrans
	connect(ui->actionPleinEcran, SIGNAL(triggered()), this, SLOT(fullscreen()));
	
	// Création de la barre d'outils
	QLabel *labelWidth = new QLabel("Diametre : "); 
	QSpinBox *spinWidth = new QSpinBox();
		spinWidth->setRange(1,20);
		spinWidth->setValue(5);
	QLabel *labelColor = new QLabel("Couleur : ");
	buttonColor = new QPushButton(); 
		buttonColor->setStyleSheet("background-color: #000000");
	QLabel *labelEraser = new QLabel("Gomme : ");
	QPushButton *buttonEraser = new QPushButton();
	QPixmap pixmap("eraser.png"); QIcon icon(pixmap); buttonEraser->setIcon(icon);
	ui->toolBar->addWidget(labelWidth); ui->toolBar->addWidget(spinWidth);
	ui->toolBar->addWidget(labelColor); ui->toolBar->addWidget(buttonColor);
	ui->toolBar->addWidget(labelEraser); ui->toolBar->addWidget(buttonEraser);
	
	// Connexion des signaux de la barre d'outils
	connect(spinWidth, SIGNAL(valueChanged(int)), drawingBoard, SLOT(changeWidth(int)));
	connect(buttonColor, SIGNAL(clicked()), this, SLOT(changeColor()));
	connect(buttonEraser, SIGNAL(clicked()), this, SLOT(toolRubber()));
	
	
	// Création de la zone centrale
	scroll = new QScrollArea();
	QWidget *mainWidget = new QWidget();
	QHBoxLayout *layout = new QHBoxLayout();
	mainWidget->setLayout(layout);
	
	// Ajout des widgets
	layout->addWidget(drawingBoard);
	layout->addWidget(camWidget);
	
	// Zone de défilement
	scroll->setWidget(mainWidget);
	this->setCentralWidget(scroll);
	
	// Démarrage du timer
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
	timer->start(40);
}

void Client::exportDraw()
{
	// Filtre sur les extensions possibles
	QString filter;
	filter += "Images (*.png)";
	
	QString folder = QDir::homePath() + "/image.png";
	
	// L'option DontUseNativeDialog empêche l'apparition d'un bug grisant la fenêtre
	QString fichier = QFileDialog::getSaveFileName(this, "Ouvrir un fichier", folder, 
				filter, new QString(), QFileDialog::DontUseNativeDialog);
	
	// Exporter en pixmap d'abord
	QPixmap pixmap = QPixmap::grabWidget(drawingBoard);
	pixmap.save(fichier);
}

void Client::flushScene()
{
	drawingBoard->flushScene();
}

void Client::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape && this->isFullScreen())
	{
		fullscreen();
	}
}

void Client::fullscreen()
{
	if (this->isFullScreen())
	{
		// Passage en mode normal
		scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		camWidget->show();
		ui->menubar->setVisible(true);
		
		// Passage en mode normal, puis maximisé 
		this->showNormal();
		this->showMaximized();
	}
	else
	{
		// Passage en mode plein écran
		ui->menubar->setVisible(false);
		camWidget->hide();
		scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		this->showFullScreen();
	}
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

void Client::changeColor()
{
	// Demande de nouvelle couleur
	QColor c = QColorDialog::getColor(QColor("#000000"));
	
	// Si la couleur est valide
	if (c.isValid())
	{
		// On colore le bouton et on change l'état
		buttonColor->setStyleSheet("background-color: " + c.name());
		drawingBoard->changeColor(c);
	}
}

void Client::toolRubber()
{
	QColor c("#FFFFFF");
	drawingBoard->changeColor(c);
}

