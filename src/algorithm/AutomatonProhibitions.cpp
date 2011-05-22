#include "AutomatonProhibitions.h"
#include "automaton-prohibitions/AddProhibitionAutomatonStateRet.h"
#include "automaton-prohibitions/ProhibitionAutomatonBuilder.h"
#include "automaton-prohibitions/ProhibitionAutomatonState.h"
#include "../IndexedSet.h"
#include "../Utils.h"

#include <deque>
#include <vector>


struct AutomatonReturnStrategy {

	typedef AutomatonProhibitions::AutomatonProhibitionsRet result_t;

	typedef ProhibitionAutomatonBuilder builder_t;

	static const bool earlyRet = false;

	static result_t getResult(bool hasProhibitions, const ProhibitionAutomaton & prohibitionAutomaton) {
		return result_t(hasProhibitions, prohibitionAutomaton);
	}
};


struct BooleanReturnStrategy {

	typedef bool result_t;

	static const bool earlyRet = true;

	typedef class Builder {
	public:

		Builder(const DeterministicTransducerAutomaton & sourceAutomaton): states() { }

		AddProhibitionAutomatonStateRet addState(const ProhibitionAutomatonState & state) {
			auto ret = states.insert(state);
			return AddProhibitionAutomatonStateRet(ret.inserted, ret.index);
		}

		const ProhibitionAutomatonState & getStateByIndex(int index) const {
			return states.get(index);
		}

		void setTransition(int sourceIndex, int sourceAutomatonOutput, int targetIndex) { }

		int getResult() const { return 42; /* dummy */ }

	private:

		IndexedSet<ProhibitionAutomatonState> states;

	} builder_t;

	static result_t getResult(bool hasProhibitions, int _) {
		return hasProhibitions;
	}
};


template <typename ReturnStrategy>
typename ReturnStrategy::result_t findAutomatonProhibitions(
	const DeterministicTransducerAutomaton & sourceAutomaton)
{
	bool hasProhibitions = false;
	typename ReturnStrategy::builder_t builder(sourceAutomaton);
	std::deque<int> indexesToProcess;

	/* добавляем состояние, не содержащее ни одного состояния исходного автомата */
	int indexEmpty = builder.addState(ProhibitionAutomatonState(ProhibitionAutomatonState::sourcestateset_t())).index;
	FOREACH_RANGE(int, i, sourceAutomaton.outputSetSize)
		builder.setTransition(indexEmpty, i, indexEmpty);
	FOREACH_END()

	/* добавляем исходное состояние автомата запретов - q0{зап},
	 * содержащее все состояния исходного автомата */
	ProhibitionAutomatonState::sourcestateset_t allSourceStates;
	FOREACH_RANGE(int, i, sourceAutomaton.stateSetSize)
		allSourceStates.insert(i);
	FOREACH_END()
	auto ret = builder.addState(ProhibitionAutomatonState(allSourceStates));
	if (ret.inserted)
		indexesToProcess.push_back(ret.index);

	while (!indexesToProcess.empty()) {
		int index = indexesToProcess.front();
		indexesToProcess.pop_front();
		const ProhibitionAutomatonState & state = builder.getStateByIndex(index);

		/* новые состояния, индексированные выходом */
		std::vector<ProhibitionAutomatonState::sourcestateset_t> newStatesData(sourceAutomaton.outputSetSize);

		FOREACH_RANGE(int, input, sourceAutomaton.inputSetSize)
			FOREACH(int, sourceState, state.sourceStates())
				DeterministicTransducerAutomaton::Transition t = sourceAutomaton.transition(sourceState, input);
				newStatesData[t.output].insert(t.state);
			FOREACH_END()
		FOREACH_END()

		FOREACH_RANGE(int, output, sourceAutomaton.outputSetSize)

			ProhibitionAutomatonState state(newStatesData[output]);
			hasProhibitions = hasProhibitions || state.empty();
			if (ReturnStrategy::earlyRet && hasProhibitions)
				return ReturnStrategy::getResult(hasProhibitions, builder.getResult());

			auto ret = builder.addState(state);
			builder.setTransition(index, output, ret.index);
			if (ret.inserted) indexesToProcess.push_back(ret.index);

		FOREACH_END()
	}

	return ReturnStrategy::getResult(hasProhibitions, builder.getResult());
}


/* static */ AutomatonProhibitions::AutomatonProhibitionsRet AutomatonProhibitions::findAutomatonProhibitions(
	const DeterministicTransducerAutomaton & sourceAutomaton) {
	return ::findAutomatonProhibitions<AutomatonReturnStrategy>(sourceAutomaton);
}

/* static */ bool AutomatonProhibitions::automatonHasProhibitions(
	const DeterministicTransducerAutomaton & sourceAutomaton) {
	return ::findAutomatonProhibitions<BooleanReturnStrategy>(sourceAutomaton);
}
