#pragma once

#include "ProhibitionAutomatonStateIndexed.h"
#include "../../DeterministicRecognizerAutomaton.h"
#include <cassert>
#include <vector>
#include <set>

class ProhibitionAutomaton: public DeterministicRecognizerAutomaton
{
public:

	typedef std::set<ProhibitionAutomatonStateIndexed> stateset_t;
	typedef std::vector<const ProhibitionAutomatonStateIndexed *> statesbyindex_t;

public:

	ProhibitionAutomaton(int inputSetSize, const IntegerFunction & transitionFunction,
		const stateset_t & states, const statesbyindex_t & statesByIndex):
		DeterministicRecognizerAutomaton(inputSetSize, static_cast<int>(states.size()), transitionFunction,
			acceptingstates_t(states.size(), true)), // все состояния являются допускающими
		states(states), statesByIndex(statesByIndex)
	{
		assert(states.size() == statesByIndex.size());
	}

	int findState(const ProhibitionAutomatonState::sourcestateset_t & sourceStates) const {
		return findState(ProhibitionAutomatonState(sourceStates));
	}

	int findState(const ProhibitionAutomatonState & state) const {
		auto it = states.find(ProhibitionAutomatonStateIndexed(-1, state));
		return it == states.end() ? -1 : it->index;
	}

	bool hasState(const ProhibitionAutomatonState::sourcestateset_t & sourceStates) const {
		return findState(sourceStates) >= 0;
	}

	bool hasState(const ProhibitionAutomatonState & state) const {
		return findState(state) >= 0;
	}

	bool hasTransition(int sourceState, int input, int targetState) const {
		return transition(sourceState, input) == targetState;
	}

private:

	stateset_t states;
	statesbyindex_t statesByIndex;
};
