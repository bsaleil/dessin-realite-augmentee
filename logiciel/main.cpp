#include <QApplication>
#include "client.hpp"

int main(int argc, char**argv)
{
	QApplication app(argc,argv);
	Client c;
	c.showMaximized();
	
	return app.exec();
}
