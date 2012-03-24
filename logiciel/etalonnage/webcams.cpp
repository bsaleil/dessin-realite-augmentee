#include "webcams.hpp"

Webcams::Webcams() : QDialog()
{
	step = 1;
	// Etat de l'étalonnage :
	webcamChoisie = -1;
	choixWebcams = NULL;

	// Layout principal
	QVBoxLayout *layoutPrincipal = new QVBoxLayout();
		QLabel *labelPrincipal = new QLabel(QString::fromUtf8("Étalonnage :"));
			QFont font; font.setPointSize(20); labelPrincipal->setFont(font);
        		labelPrincipal->setAlignment(Qt::AlignCenter);
		QSpacerItem *espaceHaut = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
		layoutCentral = new QVBoxLayout();
		QSpacerItem *espaceBas = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
		QGridLayout *layoutBas = new QGridLayout();
			QSpacerItem *espaceBasGauche = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
			boutonPrecedent = new QPushButton("<- Precedent");
			boutonSuivant = new QPushButton("Suivant ->");
	
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

void Webcams::loadWebcamsStep(int oldStep)
{
	clearLayout(layoutCentral);
	
	QLabel *labelWebcams = new QLabel("Choisissez une webcam :");
	choixWebcams = new QComboBox();
	
	layoutCentral->addWidget(labelWebcams);
	layoutCentral->addWidget(choixWebcams);
	
	WebcamManager wm;
	int nb = wm.getNumberOfWebcams();
	for (int i=0; i<nb; i++)
		choixWebcams->addItem("Webcam " + QString::number(i));
	
	boutonPrecedent->setEnabled(false);
}

void Webcams::loadClicksStep(int oldStep)
{
	pointChoisiA = new QPoint(-1,-1);
	pointChoisiB = new QPoint(-1,-1);
	// Si l'on vient de choisir la webcam
	if (oldStep == 1)
		webcamChoisie = choixWebcams->currentIndex();
	
	clearLayout(layoutCentral);
	
	QLabel *labelClicks = new QLabel("Cliquez deux fois pour selectionner votre objet :");
	
	ViewEtalonnage *viewImage = new ViewEtalonnage(pointChoisiA,pointChoisiB);
	WebcamManager wm;
	
	imageCapturee = wm.getImageInit(webcamChoisie);
	viewImage->addPixmap(QPixmap::fromImage(iplToQimage(imageCapturee)));
	
	QPushButton *boutonRecommencer = new QPushButton("Reprendre photo");
	connect(boutonRecommencer, SIGNAL(clicked()), this, SLOT(slotNewPhoto()));
	
	layoutCentral->addWidget(labelClicks);
	layoutCentral->addWidget(viewImage);
	layoutCentral->addWidget(boutonRecommencer);
	
	boutonSuivant->setText("Suivant ->");
	boutonPrecedent->setEnabled(true);
}

void Webcams::loadSettingsStep(int oldStep)
{
	// Si étalonnage pas fait/pas fini
	if (pointChoisiA->x() == -1 || pointChoisiB->x() == -1)
	{
		QMessageBox::critical(this, "Erreur", "L'étalonnage n'est pas effectué. Cliquez deux fois pour encadrer votre objet.");
		step = 2;
		loadClicksStep(2);
	}
	else
	{
		clearLayout(layoutCentral);
	
		QLabel *labelSettings = new QLabel("Ajustez le reperage de l'objet :");
		QLabel *imageSettings = new QLabel("image binarisee MAJ");
		QSlider *sliderSettings = new QSlider(Qt::Horizontal);
	
		layoutCentral->addWidget(labelSettings);
		layoutCentral->addWidget(imageSettings);
		layoutCentral->addWidget(sliderSettings);
	
		boutonSuivant->setText("Demarrer");
	}
}

void Webcams::clearLayout(QLayout *layout)
{
	QLayoutItem *item;
	while((item = layout->takeAt(0)))
	{
		if (item->layout())
		{
			clearLayout(item->layout());
			delete item->layout();
		}
		if (item->widget())
		{
			delete item->widget();
		}
		delete item;
	}
}

void Webcams::slotNewPhoto()
{
	loadClicksStep(2);
}

void Webcams::slotPrevious()
{
	if (step == 2)
	{
		step = 1; loadWebcamsStep(2);
	}
	else if (step == 3)
	{
		step = 2; loadClicksStep(3);
	}
}
void Webcams::slotNext()
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
		std::cout << "Demarrer" << std::endl;
	}
}

QImage Webcams::iplToQimage(IplImage* image)
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
