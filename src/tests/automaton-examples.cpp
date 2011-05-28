#include "automaton-examples.h"

// digraph{ }
DeterministicTransducerAutomaton createEmpty() {
	return DeterministicTransducerAutomaton(0, 0, 0,
		IntegerFunction(std::vector<int>()), IntegerFunction(std::vector<int>()));
}

// digraph{ 0 }
DeterministicTransducerAutomaton createSingleStateNoTransitions() {
	return DeterministicTransducerAutomaton(0, 0, 1,
		IntegerFunction(std::vector<int>()), IntegerFunction(std::vector<int>()));
}

// digraph{ 0 -> 0 }
DeterministicTransducerAutomaton createSingleStateSingleTransition() {
	return DeterministicTransducerAutomaton(1, 1, 1,
		IntegerFunction(std::vector<int>(1, 0)), IntegerFunction(std::vector<int>(1, 0)));
}

// digraph{ 0->0[label='0:0']; 0->0[label='1:0'] }
DeterministicTransducerAutomaton createSingleStateInformationLoss() {
	return DeterministicTransducerAutomaton(2, 1, 1,
		IntegerFunction(std::vector<int>(2, 0)), IntegerFunction(std::vector<int>(2, 0)));
}

// digraph{ 0->1[label='0:0']; 1->0[label='0:0'] }
DeterministicTransducerAutomaton createTwoStatesCycle() {
	return DeterministicTransducerAutomaton(1, 1, 2,
		IntegerFunction({ 1, 0 }),
		IntegerFunction({ 0, 0 }));
}

// digraph{ 0->1[label='0:0']; 1->0[label='0:1'] }
DeterministicTransducerAutomaton createTwoStatesWithProhibition() {
	return DeterministicTransducerAutomaton(1, 2, 2,
		IntegerFunction({ 1, 0 }),
		IntegerFunction({ 0, 1 }));
}

DeterministicTransducerAutomaton createOriginalExample() {
	return DeterministicTransducerAutomaton(2, 2, 4,
		// (state, input) -> state, output
		IntegerFunction({ 0, 1, 2, 1, 2, 1, 0, 1 }),
		IntegerFunction({ 0, 0, 1, 0, 1, 0, 0, 0 }));
}

DeterministicTransducerAutomaton createProhibitionExample() {
	return DeterministicTransducerAutomaton(2, 2, 4,
		// (state, input) -> state, output
		IntegerFunction({ 2, 3, 2, 1, 0, 1, 0, 3 }),
		IntegerFunction({ 1, 1, 1, 0, 0, 0, 0, 1 }));
}

DeterministicTransducerAutomaton createFiniteDelayExample() {
	return DeterministicTransducerAutomaton(2, 4, 5,
		// (state, input) -> state, output
		IntegerFunction({ 1, 2,  3, 4,  0, 1,  4, 4,  2, 2 }),
		IntegerFunction({ 0, 0,  0, 0,  1, 1,  2, 3,  2, 3 }));
}

DeterministicTransducerAutomaton createFiniteMaxPreimageCountExample() {
	return DeterministicTransducerAutomaton(2, 2, 8,
		// (state, input) -> state, output
		IntegerFunction({ 0,4,  0,4,  1,5,  1,5,  2,6,  2,6,  3,7,  3,7 }),
		IntegerFunction({ 0,1,  0,1,  0,1,  1,0,  0,1,  1,0,  0,1,  1,0 }));
}

DeterministicTransducerAutomaton createInfiniteDelayExample() {
	return DeterministicTransducerAutomaton(2, 2, 4,
		// (state, input) -> state, output
		IntegerFunction({ 0, 2, 0, 3, 1, 3, 1, 2 }),
		IntegerFunction({ 0, 0, 0, 1, 1, 1, 1, 0 }));
}
