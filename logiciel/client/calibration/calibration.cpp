#include "calibration.hpp"

Calibration::Calibration() : QWidget()
{
	step = 1;
	
	// Etat de l'étalonnage :
	webcamChoisie = -1;
	tracking = ""; 
	choixWebcams = NULL;
	trackingChoice = NULL;

	// Layout principal
	QVBoxLayout *layoutPrincipal = new QVBoxLayout();
		QLabel *labelPrincipal = new QLabel(tr("Configuration :"));
			QFont font; font.setPointSize(20); labelPrincipal->setFont(font);
        		labelPrincipal->setAlignment(Qt::AlignCenter);
		QSpacerItem *espaceHaut = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
		layoutCentral = new QVBoxLayout();
		QSpacerItem *espaceBas = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
		QGridLayout *layoutBas = new QGridLayout();
			QSpacerItem *espaceBasGauche = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
			boutonPrecedent = new QPushButton(tr("<- Precedent"));
			boutonSuivant = new QPushButton(tr("Suivant ->"));
	
	// Boutons suivants et précédents
	layoutBas->addItem(espaceBasGauche,0,0,1,1);
	layoutBas->addWidget(boutonPrecedent,0,1,1,1);
	layoutBas->addWidget(boutonSuivant,0,2,1,1);
	
	layoutPrincipal->addWidget(labelPrincipal);
	layoutPrincipal->addItem(espaceHaut);
	layoutPrincipal->addLayout(layoutCentral);
	layoutPrincipal->addItem(espaceBas);
	layoutPrincipal->addLayout(layoutBas);
	
	setLayout(layoutPrincipal);
	loadWebcamsStep(0);
	
	connect(boutonPrecedent, SIGNAL(clicked()), this, SLOT(slotPrevious()));
	connect(boutonSuivant,SIGNAL(clicked()), this, SLOT(slotNext()));
}

void Calibration::loadWebcamsStep(int oldStep)
{
	clearLayout(layoutCentral);
	
	QLabel *labelWebcams = new QLabel(tr("Choisissez une webcam :"));
	choixWebcams = new QComboBox();
	
	layoutCentral->addWidget(labelWebcams);
	layoutCentral->addWidget(choixWebcams);
	
	WebcamManager wm;
	int nb = wm.getNumberOfWebcams();
	for (int i=0; i<nb; i++)
		choixWebcams->addItem(tr("Webcam ") + QString::number(i));
		
	// Choix de la méthode de Tracking
	QLabel *labelTracking = new QLabel(tr("Choisissez le type de suivi :"));
	trackingChoice = new QComboBox();
	
	layoutCentral->addWidget(labelTracking);
	layoutCentral->addWidget(trackingChoice);
	
	// Types de trackings : couleur, forme, blob
	trackingChoice->addItem(tr("Couleur"));
	trackingChoice->addItem(tr("Forme"));
	trackingChoice->addItem(tr("Blob"));
	
	if (tracking == tr("Couleur")) trackingChoice->setCurrentIndex(0);
	else if (tracking == tr("Forme")) trackingChoice->setCurrentIndex(1);
	else if (tracking == tr("Blob")) trackingChoice->setCurrentIndex(2);
	
	// Empêcher de revenir en arrière
	boutonPrecedent->setEnabled(false);
}

void Calibration::loadClicksStep(int oldStep)
{
	pointChoisiA = new QPoint(-1,-1);
	pointChoisiB = new QPoint(-1,-1);
	
	// Si l'on vient de choisir la webcam et le track
	if(oldStep == 1)
	{
		// Sauvegarde des valeurs
		webcamChoisie = choixWebcams->currentIndex();
		tracking = trackingChoice->currentText();
	}
			
	clearLayout(layoutCentral);
	
	QLabel *labelClicks = new QLabel(tr("Cliquez deux fois pour selectionner votre objet :"));
	
	ViewCalibration *viewImage = new ViewCalibration(pointChoisiA,pointChoisiB);
	WebcamManager wm;
	
	imageCapturee = wm.getImageInit(webcamChoisie);
	cvFlip(imageCapturee, imageCapturee, 1);
	
	viewImage->addPixmap(QPixmap::fromImage(iplToQimage(imageCapturee)));
	
	QPushButton *boutonRecommencer = new QPushButton(tr("Reprendre photo"));
	connect(boutonRecommencer, SIGNAL(clicked()), this, SLOT(slotNewPhoto()));
	
	layoutCentral->addWidget(labelClicks);
	layoutCentral->addWidget(viewImage);
	layoutCentral->addWidget(boutonRecommencer);
	
	boutonSuivant->setText(tr("Suivant ->"));
	boutonPrecedent->setEnabled(true);
}

void Calibration::loadSettingsStep(int oldStep)
{
	// Si étalonnage pas fait/pas fini
	if (pointChoisiA->x() == -1 || pointChoisiB->x() == -1)
	{
		QMessageBox::critical(this, tr("Erreur"), tr("L'étalonnage n'est pas effectué. Cliquez deux fois pour encadrer votre objet."));
		step = 2;
		loadClicksStep(2);
	}
	// L'étalonnage a bien été effectué.
	else
	{
		CvPoint a; a.x = pointChoisiA->x(); a.y = pointChoisiA->y();
		CvPoint b; b.x = pointChoisiB->x(); b.y = pointChoisiB->y();
		
		// Choix de l'étalonnage
		if(tracking == tr("Couleur"))
			curseur = calibration(imageCapturee, a, b, TRACK_COLOR);
		
		if(tracking == tr("Forme"))
			curseur = calibration(imageCapturee, a, b, TRACK_SHAPE);
			
		if(tracking == tr("Blob"))
			curseur = calibration(imageCapturee, a, b, TRACK_BLOB);
		
		clearLayout(layoutCentral);
	
		QLabel *labelSettings;
		if (tracking == tr("Forme"))
			labelSettings = new QLabel(tr("Objet choisi :"));
		else
			labelSettings = new QLabel(tr("Ajustez le reperage de l'objet :"));
		imageSettings = new QLabel("");
			imageSettings->setPixmap(QPixmap::fromImage(iplToQimage(curseur->mask)));
			labelSlider = new QLabel(tr("Seuil : ") + QString::number(curseur->threshold));
			QSlider *sliderSettings = new QSlider(Qt::Horizontal);
				sliderSettings->setMinimum(0); sliderSettings->setMaximum(50); sliderSettings->setValue(10);
		
		layoutCentral->addWidget(labelSettings);
		layoutCentral->addWidget(imageSettings);
		
		if (tracking != tr("Forme"))
		{
			layoutCentral->addWidget(labelSlider);
			layoutCentral->addWidget(sliderSettings);
		}
	
		connect(sliderSettings,SIGNAL(valueChanged(int)),this,SLOT(slotSliderSettings(int)));
	}
}

void Calibration::loadNetworkStep(int oldStep)
{
	clearLayout(layoutCentral);
	
	QRadioButton *boutonLocal = new QRadioButton(tr("Je veux dessiner seul.")); boutonLocal->toggle();
	boutonDistant = new QRadioButton(tr("Je veux dessiner avec d'autres personnes : "));
	
	QLabel *labelAdresse = new QLabel(tr("Adresse/Nom du serveur : "));
	editAdresse = new QLineEdit("localhost");
	QLabel *labelPort = new QLabel(tr("Port : "));
	spinPort = new QSpinBox(); spinPort->setMaximum(65535); spinPort->setValue(34000);
	QSpacerItem *espaceReseau = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
	
	QHBoxLayout *layoutReseau = new QHBoxLayout();
	layoutReseau->addItem(espaceReseau);
	layoutReseau->addWidget(labelAdresse); layoutReseau->addWidget(editAdresse);
	layoutReseau->addWidget(labelPort); layoutReseau->addWidget(spinPort);
	
	layoutCentral->addWidget(boutonLocal);
	layoutCentral->addWidget(boutonDistant);
	layoutCentral->addLayout(layoutReseau);
	
	boutonSuivant->setText(tr("Demarrer"));
}

void Calibration::slotSliderSettings(int value)
{
	// Modification de la valeur affichée
	labelSlider->setText(tr("Seuil : ") + QString::number(value));
	
	// Appel de la fonction pour la nouvelle image
	curseur->threshold = value;
	binarisation(imageCapturee,curseur);
	
	// Modification de l'image affichée
	imageSettings->setPixmap(QPixmap::fromImage(iplToQimage(curseur->mask)));
}

void Calibration::clearLayout(QLayout *layout)
{
	QLayoutItem *item;
	while((item = layout->takeAt(0)))
	{
		if (item->layout())
		{
			clearLayout(item->layout());
			layout->removeItem(item);
		}
		if (item->widget())
		{
			layout->removeItem(item);
		}
		delete item->layout();
		delete item->widget();
	}
}

void Calibration::slotNewPhoto()
{
	loadClicksStep(2);
}

void Calibration::slotPrevious()
{
	if (step == 2)
	{
		step = 1; loadWebcamsStep(2);
	}
	else if (step == 3)
	{
		step = 2; loadClicksStep(3);
	}
	else if (step == 4)
	{
		step = 3; loadSettingsStep(4);
	}
}
void Calibration::slotNext()
{
	if (step == 1)
	{
		step = 2; loadClicksStep(1);
	}
	else if (step == 2)
	{
		step = 3; loadSettingsStep(2);
	}
	else if (step == 3)
	{
		step = 4; loadNetworkStep(3);
	}
	else if (step == 4)
	{
		launchClient();
	}
}

void Calibration::launchClient()
{
	if (boutonDistant->isChecked())
	{
		// TODO : Envois packet au serveur pour tester présence du serveur ?
		// Si présent on lance l'appli, sinon on redonne l'étape 4.
		Client *a = new Client(webcamChoisie,imageCapturee,*curseur,editAdresse->text(),spinPort->value());
		a->showMaximized();
		this->close();
	}
	else
	{
		Client *a = new Client(webcamChoisie,imageCapturee,*curseur);
		a->showMaximized();
		this->close();
	}
}

QImage Calibration::iplToQimage(IplImage* image)
{
	int h = image->height;
	int w = image->width;
	int channels = image->nChannels;
	QImage qimg(w, h, QImage::Format_ARGB32);
	char *data = image->imageData;
	
	for (int y = 0; y < h; y++, data += image->widthStep)
	{
		for (int x = 0; x < w; x++)
		{
			char r = 0;
			char g = 0;
			char b = 0;
			char a = 0;
			if (channels == 1)
			{
				r = data[x * channels];
				g = data[x * channels];
				b = data[x * channels];
			}
			else if (channels == 3 || channels == 4)
			{
				r = data[x * channels + 2];
				g = data[x * channels + 1];
				b = data[x * channels];
			}

			if (channels == 4)
			{
				a = data[x * channels + 3];
				qimg.setPixel(x, y, qRgba(r, g, b, a));
			}
			else
			{
				qimg.setPixel(x, y, qRgb(r, g, b));
			}
		}
	}
	return qimg;
}
