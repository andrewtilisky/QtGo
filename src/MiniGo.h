#ifndef MINIGO_H
#define MINIGO_H

#include "ui_MiniGo.h"
#include "State.h"
#include "boardview.h"

#include <QtWidgets/QMainWindow>
#include <QtGui/QPainter>
//#include <QtGui/QPaintEvent>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/qmessagebox.h>

#include <climits>

class MiniGo: public QMainWindow {
Q_OBJECT

public:
	MiniGo(QWidget *parent, unsigned char dimensions, unsigned char mPlies);
	~MiniGo();

private:
	Ui::MiniGoClass ui;
	QBrush mBrush;

	BoardView *custom_view;

	struct GameTreeNode {
		State* game_state;
		short minmax_heuristic;
		boost::ptr_vector<State> subnodes;
	};

//	GameTreeNode mActive_state;
	GameTreeNode* primary_node;

	unsigned char mPlies;
	bool mPass;

	void updateListWidget();

	//grow the game tree with n levels
	void growGameTree(unsigned char levels);
	void generateNewLevel(unsigned char level);

	void captureTest1();
	void captureTest2();

public slots:
//    void on_previewButton_clicked();
	void on_moveButton_clicked();
	void onListWidgetCurrentItemChanged(QListWidgetItem * item);
	void on_passButton_clicked();

protected:
	void paintEvent(QPaintEvent *);
};

#endif // MINIGO_H