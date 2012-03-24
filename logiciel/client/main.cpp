#include <QApplication>
#include "client.hpp"
#include "calibration.hpp"

int main(int argc, char**argv)
{
	QApplication app(argc,argv);
	
	Calibration c;
	c.show();
	
	return app.exec();
}
