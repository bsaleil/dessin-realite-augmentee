#include <QApplication>
#include "client.hpp"
#include "webcams.hpp"

int main(int argc, char**argv)
{
	QApplication app(argc,argv);
	//Client c;
	//c.showMaximized();
	
	Webcams w;
	w.show();
	
	return app.exec();
}
