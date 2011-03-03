#pragma once

#include "ProhibitionAutomatonBuilder.h"
#include "ProhibitionAutomatonState.h"
#include "../DeterministicTransducerAutomaton.h"
#include "../Utils.h"

#include <deque>
#include <vector>

class Algorithm {
public:

	static bool automatonHasProhibitions(const DeterministicTransducerAutomaton & sourceAutomaton) {
		return findAutomatonProhibitions(sourceAutomaton, false).hasProhibitions;
	}


	struct AutomatonProhibitionsRet {

		AutomatonProhibitionsRet(bool hasProhibitions, const ProhibitionAutomaton & prohibitionAutomaton):
			hasProhibitions(hasProhibitions), automaton(prohibitionAutomaton) {}

		const bool hasProhibitions;
		const ProhibitionAutomaton automaton;
	};

	static AutomatonProhibitionsRet findAutomatonProhibitions(const DeterministicTransducerAutomaton & sourceAutomaton,
		bool needProhibitionsAutomaton = true)
	{
		bool hasProhibitions = false;
		ProhibitionAutomatonBuilder builder(sourceAutomaton);
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
				FOREACH(int sourceState, state.sourceStates())
					DeterministicTransducerAutomaton::Transition t = sourceAutomaton.transition(sourceState, input);
					newStatesData[t.output].insert(t.state);
				FOREACH_END()
			FOREACH_END()

			FOREACH_RANGE(int, output, sourceAutomaton.outputSetSize)

				ProhibitionAutomatonState state(newStatesData[output]);
				hasProhibitions = hasProhibitions || state.empty();

				/* TODO: если требуется лишь определить, имеет ли автомат задержки,
				 * здесь можно возвратить false, если newStatesData[output].empty() */

				auto ret = builder.addState(state);
				builder.setTransition(index, output, ret.index);
				if (ret.inserted) indexesToProcess.push_back(ret.index);

			FOREACH_END()
		}

		return AutomatonProhibitionsRet(hasProhibitions, builder.getResult());
	}
};
