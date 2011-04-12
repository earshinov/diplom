#pragma once

#include "../DeterministicTransducerAutomaton.h"
#include "../Utils.h"

#include <deque>
#include <set>


class AutomatonDelayRet {
public:

	std::deque<int> delayByState;
	int automatonDelay;
};


// Приходится использовать mutable, потому что экземпляры этого класса мы помещаем внутри set,
// который выдаёт обратно только константные ссылки.  Естественно, те поля, которые участвуют в
// сравнении (метод compareTo), не должны быть mutable.
class AutomatonDelayAutomatonState {
public:

	AutomatonDelayAutomatonState(int sourceState):
		sourceState(sourceState), sourceState2(-1), parents(), children(), inCycle(false) { }

	AutomatonDelayAutomatonState(int state1, int state2):
		sourceState(state1), sourceState2(state2), parents(), children(), inCycle(false) { }

	int compareTo(const AutomatonDelayAutomatonState & other) const {
		int diff = sourceState - other.sourceState;
		if (diff != 0)
			return diff;
		return sourceState2 - other.sourceState2;
	}

	void addChild(int childIndex) const {
		children.insert(childIndex);
	}

	void addParent(int parentIndex) const {
		parents.insert(parentIndex);
	}

	const int sourceState;
	const int sourceState2; // may be -1

	mutable std::set<int> parents;
	mutable std::set<int> children;
	mutable bool inCycle;
};

IMPLEMENT_COMPARE(AutomatonDelayAutomatonState)


class AutomatonDelayCalculation {
public:

	AutomatonDelayCalculation(int nSourceStates):
		nStates(0), states(), statesByIndex(), indexesToProcess(),
		delayByState(nSourceStates, DELAY_UNKNOWN) { }

	int addStartingState(const AutomatonDelayAutomatonState & state) {
		auto insertedIter = states.insert(state).first;
		statesByIndex.push_back(&*insertedIter);
		return nStates++;
	}

	int addStateToProcess(const AutomatonDelayAutomatonState & state) {
		auto insertedIter = states.insert(state).first;
		statesByIndex.push_back(&*insertedIter);
		return nStates++;
	}

	const AutomatonDelayAutomatonState & getStateByIndex(int index) const {
		return *statesByIndex[index];
	}

	void setStateDelay(int sourceState, int delay) {
		delayByState[sourceState] = delay;
	}

	AutomatonDelayRet getResult() const {
		throw std::exception();
	}

	static const int DELAY_INF;
	static const int DELAY_UNKNOWN;

private:

	int nStates;
	std::set<AutomatonDelayAutomatonState> states;
	std::deque<const AutomatonDelayAutomatonState *> statesByIndex;
	std::deque<int> indexesToProcess;
	std::deque<int> delayByState; // integer >= 0 | DELAY_*
};


class AutomatonDelay {
public:

	static AutomatonDelayRet findAutomatonDelay(const DeterministicTransducerAutomaton & sourceAutomaton) {
		AutomatonDelayCalculation calc(sourceAutomaton.stateSetSize);

		FOREACH_RANGE(int, state, sourceAutomaton.stateSetSize)
			std::deque<std::deque<int> > targetStatesByOutput(sourceAutomaton.outputSetSize);

			FOREACH_RANGE(int, input, sourceAutomaton.inputSetSize)
				auto ret = sourceAutomaton.transition(state, input);
				targetStatesByOutput[ret.output].push_back(ret.state);
			FOREACH_END()

			bool hasChildren = false;
			FOREACH_RANGE(int, output, sourceAutomaton.outputSetSize)
				const std::deque<int> & targetStates = targetStatesByOutput[output];
				if (targetStates.size() >= 2) {
					hasChildren = true;
					handleStep1(calc, state, targetStates);
				}
			FOREACH_END()

			if (!hasChildren)
				calc.setStateDelay(state, 0);

		FOREACH_END()

		return calc.getResult();
	}

	static void handleStep1(AutomatonDelayCalculation & calc,
		int sourceState, const std::deque<int> & targetStates)
	{
		int parentIndex = calc.addStartingState(AutomatonDelayAutomatonState(sourceState));
		auto parent = calc.getStateByIndex(parentIndex);

		//TODO: связывать через parents и children
		foreach_2_tuples(targetStates.begin(), targetStates.end(), [&](int state1, int state2) {
			AutomatonDelayAutomatonState state(state1, state2);
			state.addParent(parentIndex);
			int index = calc.addStateToProcess(state);
			parent.addChild(index);
		} );
	}

	static const int DELAY_INF;
	static const int DELAY_UNKNOWN;
};
