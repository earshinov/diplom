#pragma once

#include "../DeterministicTransducerAutomaton.h"

// digraph{ }
DeterministicTransducerAutomaton createEmpty();

// digraph{ 0 }
DeterministicTransducerAutomaton createSingleStateNoTransitions();

// digraph{ 0 -> 0 }
DeterministicTransducerAutomaton createSingleStateSingleTransition();

// digraph{ 0->1[label='0:0']; 1->0[label='0:0'] }
DeterministicTransducerAutomaton createTwoStatesCycle();

// digraph{ 0->1[label='0:0']; 1->0[label='0:1'] }
DeterministicTransducerAutomaton createTwoStatesWithProhibition();

DeterministicTransducerAutomaton createOriginalExample();

DeterministicTransducerAutomaton createProhibitionExample();
