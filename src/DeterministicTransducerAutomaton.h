#pragma once

#include "IntegerFunction.h"

#include <cassert>

class DeterministicTransducerAutomaton {
public:

	struct Transition {

		Transition(int state, int output):
			state(state), output(output) {}

		const int state;
		const int output;
	};

public:

	DeterministicTransducerAutomaton(int inputSetSize, int outputSetSize, int stateSetSize,
        IntegerFunction transitionFunction, IntegerFunction outputFunction) :
        inputSetSize(inputSetSize), outputSetSize(outputSetSize), stateSetSize(stateSetSize),
        transitionFunction(transitionFunction), outputFunction(outputFunction)
    {
        assert(transitionFunction.size() == static_cast<size_t>(stateSetSize * inputSetSize));
        assert(outputFunction.size() == static_cast<size_t>(stateSetSize * inputSetSize));
    }

	Transition transition(int state, int input) const {
		int arg = state * inputSetSize + input;
		int resultingState = transitionFunction.evaluate(arg);
		int output = outputFunction.evaluate(arg);
		return Transition(resultingState, output);
	}

	const int inputSetSize;
	const int outputSetSize;
	const int stateSetSize;

private:

	IntegerFunction transitionFunction;
	IntegerFunction outputFunction;
};
