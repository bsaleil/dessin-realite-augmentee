#include "client.hpp"

Client::Client()
{
	ui.setupUi(this);
	
	wm = new WebcamManager();
	
	std::cout << wm->getNumberOfWebcams() << std::endl;
}
