#pragma once

#include "../DeterministicTransducerAutomaton.h"

class AutomatonMaxPreimageCount {
public:

	static const int PROBABLY_INF;

public:

	static int findMaxPreimageCount(const DeterministicTransducerAutomaton & automaton);
};
