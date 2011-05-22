#pragma once

#include "../../IntegerFunction.h"

class ProhibitionAutomatonTransitionFunctionBuilder {
public:

	explicit ProhibitionAutomatonTransitionFunctionBuilder(int prohibitionAutomatonInputSetSize) :
		inputSetSize(prohibitionAutomatonInputSetSize) {}

	void addState() {
		values.resize(values.size() + inputSetSize);
	}

	void setTransition(int sourceState, int input, int targetState) {
		values[sourceState * inputSetSize + input] = targetState;
	}

	IntegerFunction getResult() {
		// TODO: Check if all transitions are filled
		return IntegerFunction(std::move(values));
	}

private:

	const int inputSetSize;
	IntegerFunction::values_t values;
};
