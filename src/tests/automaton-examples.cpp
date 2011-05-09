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

// digraph{ 0->1[label='0:0']; 1->0[label='0:0'] }
DeterministicTransducerAutomaton createTwoStatesCycle() {
	int transition[] = { 1, 0 };
	int output[] = { 0, 0 };
	return DeterministicTransducerAutomaton(1, 1, 2,
		IntegerFunction(std::vector<int>(transition, transition+2)),
		IntegerFunction(std::vector<int>(output, output+2)));
}

// digraph{ 0->1[label='0:0']; 1->0[label='0:1'] }
DeterministicTransducerAutomaton createTwoStatesWithProhibition() {
	int transition[] = { 1, 0 };
	int output[] = { 0, 1 };
	return DeterministicTransducerAutomaton(1, 2, 2,
		IntegerFunction(std::vector<int>(transition, transition+2)),
		IntegerFunction(std::vector<int>(output, output+2)));
}

DeterministicTransducerAutomaton createOriginalExample() {
	int transition[] = { 0, 1, 2, 1, 2, 1, 0, 1 }; // (state, input) -> state
	int output[] = { 0, 0, 1, 0, 1, 0, 0, 0 };
	return DeterministicTransducerAutomaton(2, 2, 4,
		IntegerFunction(std::vector<int>(transition, transition+8)),
		IntegerFunction(std::vector<int>(output, output+8)));
}

DeterministicTransducerAutomaton createProhibitionExample() {
	int transition[] = { 2, 3, 2, 1, 0, 1, 0, 3 }; // (state, input) -> state
	int output[] = { 1, 1, 1, 0, 0, 0, 0, 1 };
	return DeterministicTransducerAutomaton(2, 2, 4,
		IntegerFunction(std::vector<int>(transition, transition+8)),
		IntegerFunction(std::vector<int>(output, output+8)));
}

DeterministicTransducerAutomaton createFiniteDelayExample() {
	int transition[] = { 1, 2,  3, 4,  0, 1,  4, 4,  2, 2,  }; // (state, input) -> state
	int output[] = { 0, 0,  0, 0,  1, 1,  2, 3,  2, 3, };
	return DeterministicTransducerAutomaton(2, 4, 5,
		IntegerFunction(std::vector<int>(transition, transition+10)),
		IntegerFunction(std::vector<int>(output, output+10)));
}

DeterministicTransducerAutomaton createFiniteMaxPreimageCountExample() {
	int transition[] = { 0,4,  0,4,  1,5,  1,5,  2,6,  2,6,  3,7,  3,7 }; // (state, input) -> state
	int output[] = { 0,1,  0,1,  0,1,  1,0,  0,1,  1,0,  0,1,  1,0 };
	return DeterministicTransducerAutomaton(2, 2, 8,
		IntegerFunction(std::vector<int>(transition, transition+16)),
		IntegerFunction(std::vector<int>(output, output+16)));
}
