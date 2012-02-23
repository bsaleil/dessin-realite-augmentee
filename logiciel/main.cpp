#include <QApplication>
#include "client.hpp"

int main(int argc, char**argv)
{
	QApplication app(argc,argv);
	Client c;
	c.show();
	
	return app.exec();
}
