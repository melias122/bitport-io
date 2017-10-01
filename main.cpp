#include <QApplication>
#include <QDebug>

#include "Config.h"
#include "Systray.h"
#include "Token.h"

Q_DECLARE_METATYPE(Token)

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	QApplication::setQuitOnLastWindowClosed(false);

	QRect crop(0, 0, 310, 358);
	QPixmap logo(":/image/black.png");
	QIcon icon = logo.copy(crop);

	app.setApplicationDisplayName("Bitport sync");
    app.setApplicationName("bitport-io");
	app.setWindowIcon(icon);

    qRegisterMetaType<Token>("Token");

	auto awe = getAwesome();
	if (!awe->initFontAwesome()) {
		qFatal("could not init awesome fonts");
		return 1;
	}
	awe->setDefaultOption("color", QColor(32, 179, 108));

	Systray tray(icon);
    return app.exec();
}
