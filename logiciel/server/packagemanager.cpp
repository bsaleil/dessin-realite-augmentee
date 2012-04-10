#include "packagemanager.hpp"

void PackageManager::sendPixmap(QDataStream &stream, QPixmap pixmap)
{
	stream << QString("item");
	stream << QString("qpixmap");
	stream << pixmap;
}

void PackageManager::sendPoint(QDataStream &stream, QPoint point)
{
	QString command("order");
	QString type("qpoint");
	
	stream << command << type;
	stream << point;
}
