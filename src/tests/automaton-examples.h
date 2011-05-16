#pragma once

#include "../DeterministicTransducerAutomaton.h"

// digraph{ }
DeterministicTransducerAutomaton createEmpty();

// digraph{ 0 }
DeterministicTransducerAutomaton createSingleStateNoTransitions();

// digraph{ 0 -> 0 }
DeterministicTransducerAutomaton createSingleStateSingleTransition();

// digraph{ 0->0[label='0:0']; 0->0[label='1:0'] }
DeterministicTransducerAutomaton createSingleStateInformationLoss();

// digraph{ 0->1[label='0:0']; 1->0[label='0:0'] }
DeterministicTransducerAutomaton createTwoStatesCycle();

// digraph{ 0->1[label='0:0']; 1->0[label='0:1'] }
DeterministicTransducerAutomaton createTwoStatesWithProhibition();

DeterministicTransducerAutomaton createOriginalExample();

DeterministicTransducerAutomaton createProhibitionExample();

DeterministicTransducerAutomaton createFiniteDelayExample();

// Автомат, соответствующий АВВП : L=3 | M=0 | f=0101010101101010
// В приложении 2 практических данных для него указано максимальное число прообразов = бескон.
// Проводя расчёты вручную, я получаю число прообразов = 8.
DeterministicTransducerAutomaton createFiniteMaxPreimageCountExample();

// Автомат, соответствующий АВВП : L=M=1 | f=00110110
DeterministicTransducerAutomaton createInfiniteDelayExample();
