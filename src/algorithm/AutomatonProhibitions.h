#pragma once

#include "../DeterministicTransducerAutomaton.h"
#include "automaton-prohibitions/ProhibitionAutomaton.h"

class AutomatonProhibitions {
public:

	struct AutomatonProhibitionsRet {

		AutomatonProhibitionsRet(bool hasProhibitions, const ProhibitionAutomaton & prohibitionAutomaton):
			hasProhibitions(hasProhibitions), automaton(prohibitionAutomaton) {}

		const bool hasProhibitions;
		const ProhibitionAutomaton automaton;
	};

	static AutomatonProhibitionsRet findAutomatonProhibitions(const DeterministicTransducerAutomaton & sourceAutomaton);

	static bool automatonHasProhibitions(const DeterministicTransducerAutomaton & sourceAutomaton);
};
