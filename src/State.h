/*
 * State.h
 *
 *  Created on: Nov 18, 2013
 *      Author: Andrew Tilisky
 */

#ifndef STATE_H_
#define STATE_H_

#include <cstdio>
#include <cstddef>

#include <QtCore/qdebug.h>

#include "Board.h"
#include "time.h"

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)
#define ABS(n) (-n)

class State {
public:
	State(unsigned char dimensions, bool black);
	State(const State& previous_state);
	virtual ~State();

	unsigned short blackScore() const;
	unsigned char dimensions() const;
//    unsigned short points() const;
	unsigned short x() const;
	unsigned short y() const;
	bool black() const;
	bool isEmpty(const unsigned char x, const unsigned char y) const;
	bool stoneColor(const unsigned char x, const unsigned char y) const;

	State* place(unsigned char x, unsigned char y);

private:
	Board* mBoard;
	bool mBlack;

	//the coordinates at which a stone was placed
	//to arrive at this function
	unsigned char mX, mY;
	unsigned short black_score, white_score;

	State(const State& previous_state, unsigned char x, unsigned char y);

	class illegalmoveexception: public exception {
		const unsigned char x, y;

		const char* what() const throw () {
			char* buff = (char*) malloc(100);
			sprintf(buff, "It's not legal to place a stone at %d, %d", x, y);
			//             std::string buffAsStdStr = buff;
			return buff;
		}
	public:
		illegalmoveexception(const unsigned char x, const unsigned char y) :
				x(x), y(y) {
		}
	};

	class substatesnotinitializedexception: public exception {
		const char* what() const throw () {
			return "The substate matrix hasn't been properly initialized";
		}
	};
};

#endif /* STATE_H_ */
