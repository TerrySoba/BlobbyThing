/*
 * GameRules.h
 *
 *  Created on: 01.04.2012
 *      Author: yoshi252
 */

#ifndef GAMESTATEMACHINE_H_
#define GAMESTATEMACHINE_H_

#include <functional>
#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <boost/foreach.hpp>

/*! \brief A state machine for use in games
 *
 *  This class implements a state machine. Transitions can be added.
 *  Transitions describe under what condition the state will change from
 *  one state to another. Transitions may contain an action that is
 *  executed when the transition is invoked.
 *
 *  The method printTransistionDebug() can be used to check if there are
 *  unreachable transitions.
 */
template <typename StateType>
class GameStateMachine {
public:
	/*! \brief Constructor
	 *
	 * \param startState the state with which the state machine starts
	 */
	GameStateMachine(StateType startState) : gameState(startState) {
		this->startState = startState;
	}

	virtual ~GameStateMachine() { }

	/*! \brief returns current state of state machine
	 *
	 * \return current state of state machine
	 */
	StateType getState() const { return gameState; }

	/*! \brief Evaluate transistions
	 *
	 *  This method evaluates if transitions are possible and executes
	 *  the first transition that is found.
	 *
	 *  \return the new state the state machine is in now
	 */
	StateType evaluate() {
		auto iterators = transitions.equal_range(getState());
		size_t count = 0;
		for (; iterators.first != iterators.second; iterators.first++) {
			count++;
			Transition& transition = iterators.first->second;
			if (transition.condition()) {
				if (transition.action) { // check if action is set
					transition.action();
				}
				gameState = transition.toState;
				break; // do not evaluate any further conditions
			}
		}
		if (count == 0) {
			// TODO: do something if no transitions exist?
		}

		return gameState;
	}

	/*! \brief add new transition to state machine
	 *
	 *  Each transition has a source and a target state.
	 *  The transition will only be taken if condition() returns
	 *  true.
	 *  If the transition is taken action() will be executed.
	 *
	 *  \param fromState the source state from which to transition
	 *  \param toState the state the transition will lead to
	 *  \param condition the condition to check before transitioning
	 *  \param action this will be executed if the transistion is taken.
	 *                May be NULL.
	 */
	void addTransition(StateType fromState,
			           StateType toState,
			           std::function<bool(void)> condition,
			           std::function<void(void)> action) {
		Transition r;
		r.fromState = fromState;
		r.toState = toState;
		r.condition = condition;
		r.action = action;
		r.id = transitions.size() + 1;
		transitions.insert(std::pair<StateType, Transition>(fromState, r));
	}


	/*! \brief Prints information about possible problems with the transitions.
	 *
	 *  Currently it checks if the transitions are reachable. If this prints
	 *  transitions, they are definitely unreachable and will never be invoked.
	 *  The inversion is not true, though. A transition may still be unreachable
	 *  although nothing is noted by this method.
	 *
	 *  \warning This method prints directly to stdout and should thereby only be
	 *           used for debugging purposes.
	 */
	void printTransitionDebug() {
		// at first set marked in every Transition to false
		BOOST_FOREACH(auto& transition, transitions) {
			transition.second.marked = false;
		}

		// recursively mark states
		recursiveMarker(startState);

		// now look for unmarked transitions. These will never be reached.
		BOOST_FOREACH(auto& transition, transitions) {
			if (!transition.second.marked) {
				std::cout << "Found unreachable transition("
						  << transition.second.id
						  << ") - from:"
						  << int(transition.second.fromState)
						  << " to:"
						  << int(transition.second.toState)
						  << std::endl;
			}
		}

	}

private:

	// used by method printTransistionDebug() to check for reachable transistions
	void recursiveMarker(StateType state) {
		auto iterators = transitions.equal_range(state);
		for (; iterators.first != iterators.second; iterators.first++) {
			Transition& transition = iterators.first->second;
			if (!transition.marked) {
				transition.marked = true;
				recursiveMarker(transition.toState);
			}
		}
	}

	struct Transition {
		StateType fromState;
		StateType toState;
		std::function<bool(void)> condition;
		std::function<void(void)> action;
		bool marked;
		size_t id;
	};

	StateType startState;
	StateType gameState;
	std::multimap<StateType, Transition> transitions;
};

#endif /* GAMESTATEMACHINE_H_ */
