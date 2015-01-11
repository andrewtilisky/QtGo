/*
 * Space.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: Andrew Tilisky
 */

#include "Point.h"

Point::Point() :
		mIs_empty(true) {
}

Point::Point(const Point& that_point) :
		mIs_empty(that_point.isEmpty()) {
	if (not mIs_empty)
		mIs_black = that_point.isBlack();
}

/*
 * Color Constructor
 *
 * Constructs a point at which a stone's been placed.
 */
Point::Point(const bool is_black) :
		mIs_empty(false), mIs_black(is_black) {
}

Point::~Point() {
}

//Point & Point::operator=(const Point& that)
//{
//        Point temporary (that);
//        return *this;
//}

bool Point::isSameColor(const bool is_black) const {
	if ( mIs_empty)
		throw pointvacantexception();

	return is_black == mIs_black;
}

bool Point::isEmpty() const {
	return mIs_empty;
}

bool Point::isBlack() const {
	if (mIs_empty)
		throw pointvacantexception();

	return mIs_black;
}

void Point::assignPiece(const bool black) {
	if (not mIs_empty)
//		return;
		throw pointoccupiedexception();

	mIs_black = black;
	mIs_empty = false;
}

void Point::capture() {
	if (mIs_empty)
		throw pointvacantexception();

	mIs_empty = true;
}
