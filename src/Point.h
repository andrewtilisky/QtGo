/*
 * Space.h
 *
 *  Created on: Nov 18, 2013
 *      Author: Andrew Tilisky
 */

#include <exception>

using std::exception;

#ifndef POINT_H_
#define POINT_H_

class Point {
public:
	Point();
	Point(const Point& obj);
	Point(bool isBlack);
	virtual ~Point();
	//    Point & operator= (const Point& that);

	bool isEmpty() const;
	bool isSameColor(bool black) const;

	bool isBlack() const;
	void assignPiece(const bool black);
	void capture();

	bool mIs_empty;
	bool mIs_black;
//private:

	class pointoccupiedexception: public exception {
		const char* what() const throw () {
			return "Tried to place a stone at an occupied point";
		}
	};

	class pointvacantexception: public exception {
		const char* what() const throw () {
			return "Tried to perform an invalid operation on a vacant point";
		}
	};
};

#endif /* POINT_H_ */
