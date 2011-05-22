#pragma once

#include "ProhibitionAutomatonState.h"
#include "../../DeterministicRecognizerAutomaton.h"
#include "../../IndexedSet.h"

class ProhibitionAutomaton: public DeterministicRecognizerAutomaton
{
public:

	typedef IndexedSet<ProhibitionAutomatonState> stateset_t;

public:

	ProhibitionAutomaton(int inputSetSize, IntegerFunction && transitionFunction, stateset_t && states):
		DeterministicRecognizerAutomaton(inputSetSize, static_cast<int>(states.size()), std::move(transitionFunction),
			acceptingstates_t(states.size(), true)), // все состояния являются допускающими
		states(std::move(states)) { }

	int findState(const ProhibitionAutomatonState & state) const {
		return states.find(state);
	}

	int findState(ProhibitionAutomatonState && state) const {
		return states.find(std::move(state));
	}

	bool hasState(const ProhibitionAutomatonState & state) const {
		return findState(state) >= 0;
	}

	bool hasState(ProhibitionAutomatonState && state) const {
		return findState(std::move(state)) >= 0;
	}

	bool hasTransition(int sourceState, int input, int targetState) const {
		return transition(sourceState, input) == targetState;
	}

private:

	stateset_t states;
};
