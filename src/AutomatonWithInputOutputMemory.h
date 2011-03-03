#pragma once

#include "BooleanFunction.h"
#include "DeterministicAutomaton.h"

#include <memory>

class AutomatonWithInputOutputMemory {
public:

	AutomatonWithInputOutputMemory(int inputMemorySize, int outputMemorySize, BooleanFunction func):
		inputMemorySize(inputMemorySize), outputMemorySize(outputMemorySize), func(func) {}

	DeterministicTransducerAutomaton & asAutomaton() const {
		if (automaton.get() == 0)
			automaton = std::auto_ptr<DeterministicTransducerAutomaton>(0);
		return *automaton.get();
	}

private:

	const int inputMemorySize;
	const int outputMemorySize;
	const BooleanFunction func;
	mutable std::auto_ptr<DeterministicTransducerAutomaton> automaton;
};
