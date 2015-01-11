#include "SetupDialog.h"

Dialog::Dialog() :
		QDialog() {
	ui.setupUi(this);
	on_okButton_clicked();
}

Dialog::~Dialog() {
	delete w;
}

void Dialog::on_okButton_clicked() {
	//goFirst = ui.goFirstBox->isChecked();
	unsigned char dimensions = ui.dimensionsBox->value();
	//min = ui.minMaxBox->currentIndex();
	unsigned char plies = ui.plyBox->value();

	const bool test = true;

	if (test) {
		dimensions = 9;
		plies = 2;
	}

	w = new MiniGo(NULL, dimensions, plies);
	w->show();
	//w.on_moveButton_clicked();
}
