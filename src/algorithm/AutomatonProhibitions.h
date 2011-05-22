#pragma once

#include "../DeterministicTransducerAutomaton.h"
#include "automaton-prohibitions/ProhibitionAutomaton.h"

class AutomatonProhibitions {
public:

	struct AutomatonProhibitionsRet {

		AutomatonProhibitionsRet(bool hasProhibitions, ProhibitionAutomaton && prohibitionAutomaton):
			hasProhibitions(hasProhibitions), automaton(std::move(prohibitionAutomaton)) {}

		const bool hasProhibitions;
		const ProhibitionAutomaton automaton;
	};

	static AutomatonProhibitionsRet findAutomatonProhibitions(const DeterministicTransducerAutomaton & sourceAutomaton);

	static bool automatonHasProhibitions(const DeterministicTransducerAutomaton & sourceAutomaton);
};
