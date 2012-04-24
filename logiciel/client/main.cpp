#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include "client.hpp"
#include "calibration.hpp"

int main(int argc, char**argv)
{
	QApplication app(argc,argv);
	
	// Detection de la locale
	QString locale = QLocale::system().name().section('_', 0, 0);
 
 	qDebug() << "client_" + locale;
 
	QTranslator translator;
	translator.load(QString("client_") + locale);
	app.installTranslator(&translator);
	
	Calibration c;
	c.show();
	
	return app.exec();
}
