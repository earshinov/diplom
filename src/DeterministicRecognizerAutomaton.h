#pragma once

#include "IntegerFunction.h"

#include <cassert>

class DeterministicRecognizerAutomaton {
public:

	DeterministicRecognizerAutomaton(int inputSetSize, int stateSetSize, IntegerFunction transitionFunction) :
        inputSetSize(inputSetSize), stateSetSize(stateSetSize), transitionFunction(transitionFunction)
    {
        assert(transitionFunction.size() == static_cast<size_t>(stateSetSize * inputSetSize));
    }

	int transition(int state, int input) const {
		int arg = state * inputSetSize + input;
		return transitionFunction.evaluate(arg);
	}

	const int inputSetSize;
	const int stateSetSize;

private:

	IntegerFunction transitionFunction;
};
