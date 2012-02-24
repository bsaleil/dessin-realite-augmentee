#include "widgetwebcam.hpp"

WidgetWebcam::WidgetWebcam()
{
	// De base, la classe n'est pas prête à l'étalonnage
	readyToCalibrate = false;
}

WidgetWebcam::WidgetWebcam(QString text) : QLabel(text)
{
	// On appelle le constructeur de QLabel pour afficher le texte
	// Puis on appelle le constructeur par défaut
	WidgetWebcam();
}

void WidgetWebcam::calibrate(IplImage* iImage)
{
	// Conversion
	QImage *qImage = iplToQimage(iImage);
	
	// Affichage
	this->setPixmap(QPixmap::fromImage(*qImage));
	this->show();
	
	// Etalonnage maintenant possible
	readyToCalibrate = true;
}

void WidgetWebcam::mousePressEvent(QMouseEvent * event)
{
	// Si la classe est prête à recevoir le clic d'étalonnage
	if (readyToCalibrate)
	{
		// TODO
		qDebug() << event->x() << "," << event->y();
		//Pixel * curseur = initNaiveColorTrack(image, x,y);
		// Ensuite si tu veux acceder
		
	}
}

QImage* WidgetWebcam::iplToQimage(IplImage* image)
{
	int h = image->height;
	int w = image->width;
	int channels = image->nChannels;
	QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);
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
				qimg->setPixel(x, y, qRgba(r, g, b, a));
			}
			else
			{
				qimg->setPixel(x, y, qRgb(r, g, b));
			}
		}
	}
	return qimg;
}