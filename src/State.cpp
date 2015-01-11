/*
 * State.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: Andrew Tilisky
 *
 * known invariants:
 *
 * 1. color cannot be changed post instantiation.
 * color indicates the player whose placement
 * generated this state.
 *
 * 2. does not allow direct access to substate vector.
 *
 * 3. A state cannot place a stone onto its board outside its
 * "copy placement constructor".  In other words, a game state
 * is created and should not be able to be modified thereafter.
 * Outside classes that "play the game" and make moves need to
 * update their State objects with the substate of the current
 * state that represents the move being made.
 */

#include "State.h"

State::State(unsigned char dimensions, bool black) :
		mBoard(new Board(dimensions)), mBlack(black) {
	//    mSubsequent_states.resize(mBoard->points(), this);
	//    mSubsequent_states = ;
	//    long runstarttime = time::GetTimeMs64();
	//    long runendtime = time::GetTimeMs64();
	//    long runtime = runendtime - runstarttime;

	//    qDebug("initial generation time %d for %d", runtime, plies);
	//    qDebug("plies dimension %d");
//	qDebug("mSubsequent_states.size() %lu", mSubsequent_states.size());

	//    if(mSubsequent_states.size() < mBoard->points())
	//        throw substatesnotinitializedexception();
}

State::State(const State& that_state) :
		mBoard(that_state.mBoard), mBlack(not that_state.mBlack) {
}

State::State(const State& previous_state, unsigned char x, unsigned char y) :
		mBoard(previous_state.mBoard), mBlack(not previous_state.black()) {
	mBoard->place(mBlack, x, y);
	mX = x, mY = y;
}

/*
 * board needs to be deleted but vector of subsequent states should be
 * cleaned up automatically by boost
 */
State::~State() {
	delete mBoard;
}

unsigned short State::blackScore() const {
	return black_score;
}

//unsigned short points() const {
//    return mDimensions * mDimensions;
//}

unsigned char State::dimensions() const {
	return mBoard->dimensions();
}

bool State::black() const {
	return mBlack;
}

unsigned short State::x() const {
	return mX;
}

unsigned short State::y() const {
	return mY;
}

bool State::stoneColor(const unsigned char x, const unsigned char y) const {
	return mBoard->isBlack(x, y);
}

/*
 * invariant:
 */
State* State::place(unsigned char x, unsigned char y) {

	if (not mBoard->placementIsLegal(not mBlack, x, y))
		throw illegalmoveexception(x, y);

	return new State(*this, x, y);
	//    mSubsequent_states.replace(mBoard->index(x, y), new State(&this, x, y));
//	return subState(x, y);
}

bool State::isEmpty(const unsigned char x, const unsigned char y) const {
	return mBoard->isEmpty(x, y);
}
