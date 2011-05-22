#pragma once

#include "AddProhibitionAutomatonStateRet.h"
#include "ProhibitionAutomaton.h"
#include "ProhibitionAutomatonTransitionFunctionBuilder.h"
#include "../../DeterministicTransducerAutomaton.h"

class ProhibitionAutomatonBuilder {
public:

	explicit ProhibitionAutomatonBuilder(const DeterministicTransducerAutomaton & sourceAutomaton):
		inputSetSize(sourceAutomaton.outputSetSize),
		states(), transitionFunctionBuilder(inputSetSize) {}

	AddProhibitionAutomatonStateRet addState(ProhibitionAutomatonState && state) {
		auto ret = states.insert(std::move(state));
		if (ret.inserted)
			transitionFunctionBuilder.addState();
		return AddProhibitionAutomatonStateRet(ret.inserted, ret.index);
	}

	void setTransition(int sourceIndex, int sourceAutomatonOutput, int targetIndex) {
		transitionFunctionBuilder.setTransition(sourceIndex,
		    sourceAutomatonOutput, targetIndex);
	}

	const ProhibitionAutomatonState & getStateByIndex(int index) const {
		return states.get(index);
	}

	ProhibitionAutomaton getResult() {
		return ProhibitionAutomaton(inputSetSize, transitionFunctionBuilder.getResult(), std::move(states));
	}

private:

	const int inputSetSize;
	ProhibitionAutomaton::stateset_t states;
	ProhibitionAutomatonTransitionFunctionBuilder transitionFunctionBuilder;
};
