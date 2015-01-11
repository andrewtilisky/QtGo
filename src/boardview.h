#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QtWidgets/qgraphicsview.h>
#include <QtWidgets/qgraphicsitem.h>
#include <QtGui/qevent.h>
#include <QtCore/qdebug.h>

#include <iostream>

#include "State.h"

using namespace std;

class BoardView: public QGraphicsView {
Q_OBJECT

public:
	explicit BoardView(QWidget *parent = 0, State* state_in_play = 0);
	void paint_board(QPaintEvent*);

	struct LastPointClicked {
		LastPointClicked() {
		}
		LastPointClicked(const unsigned char x, const unsigned char y) :
				x(x), y(y) {
		}

		unsigned char x, y;
	};

	const LastPointClicked lastPointClicked() const;
	void setState(State* state_in_play);

public slots:
	void mousePressEvent(QMouseEvent *);
	// void mouseReleaseEvent(QMouseEvent * e);
	// void mouseDoubleClickEvent(QMouseEvent * e);
	// void mouseMoveEvent(QMouseEvent * e);

private:
	//    QGraphicsScene * scene;
	State* mState_in_play;
	const unsigned char line_draw_offset;
	LastPointClicked mLast_point_clicked;

	float cell_dim() const;
	int line_width() const;
	int compute_coord(int point);
	float scale(int height, float dimensions);
	bool clicked_valid_point(const unsigned char x,
			const unsigned char y) const;

	void draw_lines();
	void drawStones();
	void drawStone(int pointx, int pointy, bool black);

	class illegalmoveexception: public exception {
		const unsigned char x, y;
	public:
		illegalmoveexception(const unsigned char x, const unsigned char y) :
				x(x), y(y) {
		}

		const char* what() const throw () {
			char* buff = (char*) malloc(100);
			sprintf(buff, "%d, %d isn't a valid coordinate.", x, y);
			//             std::string buffAsStdStr = buff;
			return buff;
		}
	};
};

#endif // BOARDVIEW_H
