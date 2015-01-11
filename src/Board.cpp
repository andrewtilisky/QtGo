/*
 * Board.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: Andrew Tilisky
 */

#include "Board.h"

Board::Board(unsigned char dimensions) :
		mDimensions(dimensions), mPoints(boost::ptr_vector<Point>()) {
	mPoints.resize(points(), new Point());
	if (mPoints.size() != points())
		throw pointsnotinitializedexception();
}

Board::Board(const Board &that) :
		mDimensions(that.mDimensions), mPoints(that.mPoints) {
}

Board::~Board() {
}

bool Board::isEmpty(unsigned char x, unsigned char y) const {
	return mPoints.at(index(x, y)).isEmpty();
}

bool Board::isBlack(const unsigned char x, const unsigned char y) const {
	return mPoints[index(x, y)].isBlack();
}

unsigned short Board::stones() const {
	return mStones;
}

unsigned short Board::blackStones() const {
	return mBlack_stones;
}

unsigned short Board::whiteStones() const {
	return mWhite_stones;
}

unsigned char Board::dimensions() const {
	return mDimensions;
}

unsigned short Board::index(unsigned char x, unsigned char y) const {
	return y * mDimensions + x;
}

unsigned short Board::points() const {
	return mDimensions * mDimensions;
}

bool Board::outOfBounds(unsigned char x, unsigned char y) const {
	return x >= mDimensions or y >= mDimensions;
}

bool Board::placementIsLegal(const bool black, const unsigned char x,
		const unsigned char y) {
	if (isEmpty(x, y)) {
//		return not willBeTakenCuda(mDimensions, mPoints.c_array(), x, y, black);
		return not willBeTakenCpu(black, x, y);
	} else
		return false;
}

void Board::printBoard() const {
	QDebug debug = qDebug();
	debug << "";
	for (unsigned char y = 0; y < mDimensions; ++y) {
		for (unsigned char x = 0; x < mDimensions; ++x)
			if (isEmpty(x, y))
				debug << '+';
			else if (isBlack(x, y))
				debug << '*';
			else
				debug << 'o';
		debug << '\n';
	}
}

///determining legality involves
//1. placing the stone,
//2. seeing which opposing stones are capturing
//3. seeing which of your stones are capturing
//4. checking the temporary board to see if the
//originally placed stone is still there
bool Board::willBeTakenCpu(const bool black, unsigned char x,
		unsigned char y) const {
	Board temporary(*this);
	qDebug("placing at %d, %d on temp board", x, y);
	temporary.place(black, x, y);

	return temporary.isEmpty(x, y);
}

void Board::place(const bool black, const unsigned char x,
		const unsigned char y) {
	++mStones;
	black ? ++mBlack_stones : ++mWhite_stones;

	qDebug("assigning %s piece to %d, %d", black?"black":"white", x, y);
	mPoints[index(x, y)].assignPiece(black);
	printBoard();

	qDebug("capturing %s stones", not black?"black":"white");
	captureStones(not black);
	printBoard();

	qDebug("capturing %s stones", black?"black":"white");
	captureStones(black);
	printBoard();
}

struct Pair {
	unsigned char x, y;
	Pair(unsigned char x, unsigned char y) :
			x(x), y(y) {

	}
};

void Board::captureStones(const bool black) {
	vector<Pair> captured_points;
//#pragma omp parallel for schedule(dynamic)
	for (unsigned char y = 0; y < mDimensions; ++y)
		for (unsigned char x = 0; x < mDimensions; ++x)
			if (not isEmpty(x, y) and isBlack(x, y) == black
					and pointWillBeCaptured(x, y, black)) {
				captured_points.push_back(Pair(x, y));
			}

	for (vector<Pair>::iterator it = captured_points.begin();
			it != captured_points.end(); ++it) {
		qDebug("capturing %d, %d", (*it).x, (*it).y);
		capture((*it).x, (*it).y);
	}
}

//returns 1 for black, 0 for white, and -1 for neither.
//checks once again if the point's empty because we're
//assuming this is an empty point and trying to find
//if it's within either player's territory.
int Board::pointWillBeCaptured(unsigned char x, unsigned char y,
		bool isBlack) const {
	if (isEmpty(x, y))
		throw Point::pointvacantexception();
	else {
		bool wereHere[points()];
		fill_n(wereHere, points(), false);
		return not hasLiberty(x, y, isBlack, wereHere);
	}
}

//returns whether or not a liberty can be found by traversing
//same-color stones.
//this is based on wikipedia's java maze example.
bool Board::hasLiberty(int x, int y, bool black, bool* wereHere) const {
	if (wereHere[index(x, y)])
		return false;

	wereHere[index(x, y)] = true;

	if (isEmpty(x, y)) {
		return true;
	} else if (isBlack(x, y) != black)
		return false;

	if (x != 0) // Checks if not on left edge
		if (hasLiberty(x - 1, y, black, wereHere))
			return true;

	if (x != dimensions() - 1) // Checks if not on right edge
		if (hasLiberty(x + 1, y, black, wereHere))
			return true;

	if (y != 0)  // Checks if not on top edge
		if (hasLiberty(x, y - 1, black, wereHere))
			return true;

	if (y != dimensions() - 1) // Checks if not on bottom edge
		if (hasLiberty(x, y + 1, black, wereHere))
			return true;

	return false;
}

//returns 1 for black, 0 for white, and -1 for neither.
//checks once again if the point's empty because we're
//assuming this is an empty point and trying to find
//if it's within either player's territory.
int Board::pointIsHeldBy(unsigned char x, unsigned char y) const {
	if (isEmpty(x, y)) {
		//starts at -1
		//white == 0
		//black == 1
		// both == 2
		char search_flag = -1;
		return discoverHolderRecursively(x, y, search_flag);
	} else
		throw Point::pointoccupiedexception();
}

//not trying to find a path just trying to
//find out IF any enemy stones are reachable.
//this is based on wikipedia's java maze example
bool Board::discoverHolderRecursively(int x, int y, char search_flag) const {
	//rationale:
	//if there's a black here and i haven't found a white
	//stone before set flag to black
	//if there's a black here and i have found a white
	//stone before set flag to black
	if (not isEmpty(x, y)) {
		if (isBlack(x, y)) {
			if (search_flag == -1) {
				search_flag = 1;
			} else if (search_flag == 0) {
				search_flag = 2;
			}
		} else {
			if (search_flag == -1) {
				search_flag = 0;
			} else if (search_flag == 1) {
				search_flag = 2;
			}
		}
		return search_flag;
	}

	if (x != 0) // Checks if not on left edge
		if (discoverHolderRecursively(x - 1, y, search_flag) == 2)
			return search_flag;

	if (x != dimensions() - 1) // Checks if not on right edge
		if (discoverHolderRecursively(x + 1, y, search_flag) == 2)
			return search_flag;

	if (y != 0)  // Checks if not on top edge
		if (discoverHolderRecursively(x, y - 1, search_flag) == 2)
			return search_flag;

	if (y != dimensions() - 1) // Checks if not on bottom edge
		if (discoverHolderRecursively(x, y + 1, search_flag) == 2)
			return search_flag;

	return search_flag;
}

void Board::capture(unsigned char x, unsigned char y) {
	--mStones;
	isBlack(x, y) ? --mBlack_stones : --mWhite_stones;
	mPoints[index(x, y)].capture();
}

unsigned short Board::territory(const bool black) const {
	unsigned short territory = 0;

#pragma omp parallel for //schedule(dynamic) reduction(+:territory)	for (unsigned char  y = 0; y < mDimensions; ++y) {		for (unsigned char  x = 0; x < mDimensions; ++x) {			if (isEmpty(x, y) and pointIsHeldBy(x, y)) {				territory++;			} else if (mPoints[index(x, y)].isSameColor(black))				territory++;		}	}	return territory;}