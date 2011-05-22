#pragma once

#include "BooleanFunction.h"
#include "DeterministicTransducerAutomaton.h"

class AutomatonWithInputOutputMemory {
public:

	AutomatonWithInputOutputMemory(int inputMemorySize, int outputMemorySize, BooleanFunction func):
		inputMemorySize(inputMemorySize), outputMemorySize(outputMemorySize), func(func) {}

	DeterministicTransducerAutomaton getAutomaton() const;

private:

	const int inputMemorySize;
	const int outputMemorySize;
	const BooleanFunction func;
};
