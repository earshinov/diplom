#pragma once

#include "IntegerFunction.h"

#include <cassert>

class DeterministicRecognizerAutomaton {
public:

	typedef std::vector<bool> acceptingstates_t;

public:

	DeterministicRecognizerAutomaton(int inputSetSize, int stateSetSize,
		IntegerFunction transitionFunction, acceptingstates_t acceptingStates) :
        inputSetSize(inputSetSize), stateSetSize(stateSetSize),
        transitionFunction(std::move(transitionFunction)), acceptingStates(std::move(acceptingStates))
    {
        assert(this->transitionFunction.size() == static_cast<size_t>(stateSetSize * inputSetSize));
        assert(this->acceptingStates.size() == static_cast<size_t>(stateSetSize));
    }

	int transition(int state, int input) const {
		int arg = state * inputSetSize + input;
		return transitionFunction.evaluate(arg);
	}

	bool isAcceptingState(int state) const {
		return acceptingStates[state];
	}

	const int inputSetSize;
	const int stateSetSize;

private:

	IntegerFunction transitionFunction;
	acceptingstates_t acceptingStates;
};
