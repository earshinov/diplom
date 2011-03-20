#pragma once

#include "AddProhibitionAutomatonStateRet.h"
#include "ProhibitionAutomaton.h"
#include "ProhibitionAutomatonStateIndexed.h"
#include "ProhibitionAutomatonTransitionFunctionBuilder.h"
#include "../../DeterministicTransducerAutomaton.h"

class ProhibitionAutomatonBuilder {
public:

	ProhibitionAutomatonBuilder(const DeterministicTransducerAutomaton & sourceAutomaton):
		stateSetSize(0), inputSetSize(sourceAutomaton.outputSetSize),
		states(), statesByIndex(), transitionFunctionBuilder(inputSetSize) {}

	AddProhibitionAutomatonStateRet addState(const ProhibitionAutomatonState & state) {
		auto result = states.insert(ProhibitionAutomatonStateIndexed(stateSetSize, state));
		bool inserted = result.second;
		int index;
		if (!inserted)
			index = result.first->index;
		else {
			index = stateSetSize;
			++stateSetSize;
			statesByIndex.push_back(&*(result.first));
			transitionFunctionBuilder.addState();
		}
		return AddProhibitionAutomatonStateRet(inserted, index);
	}

	void setTransition(int sourceIndex, int sourceAutomatonOutput, int targetIndex) {
		transitionFunctionBuilder.setTransition(sourceIndex,
		    sourceAutomatonOutput, targetIndex);
	}

	const ProhibitionAutomatonState & getStateByIndex(int index) const {
		return statesByIndex[index]->state;
	}

	ProhibitionAutomaton getResult() const {
		return ProhibitionAutomaton(inputSetSize,
			transitionFunctionBuilder.getResult(), states, statesByIndex);
	}

private:

	int stateSetSize;
	const int inputSetSize;
	ProhibitionAutomaton::stateset_t states;
	ProhibitionAutomaton::statesbyindex_t statesByIndex;
	ProhibitionAutomatonTransitionFunctionBuilder transitionFunctionBuilder;
};