#include "boardview.h"

BoardView::BoardView(QWidget *parent, State* state_in_play) :
		QGraphicsView(parent), mState_in_play(state_in_play), line_draw_offset(
				3) {
	//    setScene(new QGraphicsScene());
	//    setObjectName(QStringLiteral("graphicsView"));
	//    setGeometry(QRect(240, 10, 380, 380));
}

void BoardView::paint_board(QPaintEvent*) {
	draw_lines();
	drawStones();
}

void BoardView::drawStones() {
	for (unsigned char y = 0; y < mState_in_play->dimensions(); ++y)
		for (unsigned char x = 0; x < mState_in_play->dimensions(); ++x)
			if (not mState_in_play->isEmpty(x, y))
				drawStone(x, y, mState_in_play->stoneColor(x, y));
}

void BoardView::drawStone(int pointx, int pointy, bool black) {

	QGraphicsPixmapItem* item;
	QPixmap qPixMap;

	if (black) {
		qPixMap = QPixmap("../minigo/blackstone.png");
	} else {
		qPixMap = QPixmap("../minigo/whitestone.png");
	}

	qPixMap = qPixMap.scaledToHeight(
			scale(qPixMap.height(), mState_in_play->dimensions()));
	item = new QGraphicsPixmapItem(qPixMap);

	item->setPos(compute_coord(pointx), compute_coord(pointy));
	scene()->addItem(item);
}

int BoardView::compute_coord(int point) {
	return cell_dim() * (point + 1)
			- scale(46, mState_in_play->dimensions()) / 2;
}

float BoardView::cell_dim() const {
	return width() / (float) (mState_in_play->dimensions() + 1);
}

int BoardView::line_width() const {
	return width() - line_draw_offset;
}

void BoardView::draw_lines() {
	scene()->clear();

	//print lines
	for (int line = mState_in_play->dimensions(); line > 0; --line) {
		scene()->addLine(cell_dim() * line, 0, cell_dim() * line, line_width());
		scene()->addLine(0, cell_dim() * line, line_width(), cell_dim() * line);
	}
}

float BoardView::scale(int height, float dimensions) {
	if (dimensions < 6)
		return height;
	else
		//7 should return 1
		//19 should return 3
		return height / (dimensions / 6);
}

const BoardView::LastPointClicked BoardView::lastPointClicked() const {
	return mLast_point_clicked;
}

bool BoardView::clicked_valid_point(const unsigned char x,
		const unsigned char y) const {
	return y < mState_in_play->dimensions() and x < mState_in_play->dimensions();
}

void BoardView::setState(State* state_in_play) {
	mState_in_play = state_in_play;
}

void BoardView::mousePressEvent(QMouseEvent * e) {
	paint_board(NULL);

	int x = e->pos().x();
	int y = e->pos().y();

	x += cell_dim() / 2;
	y += cell_dim() / 2;

	int pointx = x / cell_dim();
	--pointx;
	int pointy = y / cell_dim();
	--pointy;

	if (pointx < 0 or pointy < 0)
		return;

	//    qDebug() << "x " << x;
	//    qDebug() << "y " << y << '\n';

//    qDebug("pointx %d", pointx);
//    qDebug("pointy %d", pointy);

	if (not clicked_valid_point(pointx, pointy)) {
		illegalmoveexception sillyclick(pointx, pointy);
		qDebug() << sillyclick.what();
		return;
	}

	mLast_point_clicked = LastPointClicked(pointx, pointy);
//	try {
//		mState_in_play = mState_in_play->place(mLast_point_clicked.x,
//				mLast_point_clicked.y);
//	} catch (exception& e) {
//		qDebug("%s", e.what());
//	}
	drawStone(pointx, pointy, not mState_in_play->black());
}
