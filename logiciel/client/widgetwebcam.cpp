#include "widgetwebcam.hpp"

WidgetWebcam::WidgetWebcam() : QLabel()
{
	setFixedSize(640,480);
}

QPoint WidgetWebcam::getNewPosition(IplImage *i, Cursor *c)
{
	// Appel de la librairie
	track(i, c);
	QPoint p(c->center.x, c->center.y);
	
	QPixmap pix = QPixmap::fromImage(iplToQimage(i));
	
	double w = fabs(c->cornerA.x-c->cornerB.x);
	double h = fabs(c->cornerA.y-c->cornerB.y);
	
	QPainter painter(&pix);
	painter.drawRect(c->center.x-w/2,c->center.y-h/2,w,h);
       	painter.end();
	
	setPixmap(pix);
	return p;
}

QImage WidgetWebcam::iplToQimage(IplImage* image)
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
