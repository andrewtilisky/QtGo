#include "SetupDialog.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	Dialog s;

	//comment this out to prevent the setup dialog from showing
	//if using hardcoded dimension/ply values
	//s.show();
	return a.exec();
}
