#include "widgetwebcam.hpp"

WidgetWebcam::WidgetWebcam()
{
}

WidgetWebcam::WidgetWebcam(QString text) : QLabel(text)
{
}

// TODO : récupère la coordonée du clic, en déduit la couleur issue de l'image dessous
void WidgetWebcam::mousePressEvent(QMouseEvent * event)
{


}

void WidgetWebcam::calibrate(IplImage* iImage)
{
	// Conversion
	QImage *qImage = iplToQimage(iImage);
	
	// Affichage
	this->setPixmap(QPixmap::fromImage(*qImage));
	this->show();
	
	// TODO : étalonner
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
