#pragma once

#include "ProhibitionAutomatonState.h"
#include "../../DeterministicRecognizerAutomaton.h"
#include "../../IndexedSet.h"
#include <cassert>
#include <vector>
#include <set>

class ProhibitionAutomaton: public DeterministicRecognizerAutomaton
{
public:

	typedef IndexedSet<ProhibitionAutomatonState> stateset_t;

public:

	ProhibitionAutomaton(int inputSetSize, const IntegerFunction & transitionFunction, const stateset_t & states):
		DeterministicRecognizerAutomaton(inputSetSize, static_cast<int>(states.size()),
			transitionFunction, acceptingstates_t(states.size(), true)), // все состояния являются допускающими
		states(states) { }

	int findState(const ProhibitionAutomatonState::sourcestateset_t & sourceStates) const {
		return findState(ProhibitionAutomatonState(sourceStates));
	}

	int findState(const ProhibitionAutomatonState & state) const {
		return states.find(state);
	}

	bool hasState(const ProhibitionAutomatonState::sourcestateset_t & sourceStates) const {
		return findState(sourceStates) >= 0;
	}

	bool hasState(const ProhibitionAutomatonState & state) const {
		return findState(state) >= 0;
	}

	bool hasTransition(int sourceState, int input, int targetState) const {
		return transition(sourceState, input) == targetState;
	}

private:

	stateset_t states;
};
