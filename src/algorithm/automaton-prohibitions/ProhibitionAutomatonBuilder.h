#pragma once

#include "AddProhibitionAutomatonStateRet.h"
#include "ProhibitionAutomaton.h"
#include "ProhibitionAutomatonTransitionFunctionBuilder.h"
#include "../../DeterministicTransducerAutomaton.h"

class ProhibitionAutomatonBuilder {
public:

	ProhibitionAutomatonBuilder(const DeterministicTransducerAutomaton & sourceAutomaton):
		inputSetSize(sourceAutomaton.outputSetSize),
		states(), transitionFunctionBuilder(inputSetSize) {}

	AddProhibitionAutomatonStateRet addState(const ProhibitionAutomatonState & state) {
		auto ret = states.insert(state);
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

	ProhibitionAutomaton getResult() const {
		return ProhibitionAutomaton(inputSetSize, transitionFunctionBuilder.getResult(), states);
	}

private:

	const int inputSetSize;
	ProhibitionAutomaton::stateset_t states;
	ProhibitionAutomatonTransitionFunctionBuilder transitionFunctionBuilder;
};
