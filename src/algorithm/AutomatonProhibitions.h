#pragma once

#include "automaton-prohibitions/AddProhibitionAutomatonStateRet.h"
#include "automaton-prohibitions/ProhibitionAutomatonBuilder.h"
#include "automaton-prohibitions/ProhibitionAutomatonState.h"
#include "../DeterministicTransducerAutomaton.h"
#include "../Utils.h"

#include <deque>
#include <map>
#include <vector>

class AutomatonProhibitions {
public:

	struct AutomatonProhibitionsRet {

		AutomatonProhibitionsRet(bool hasProhibitions, const ProhibitionAutomaton & prohibitionAutomaton):
			hasProhibitions(hasProhibitions), automaton(prohibitionAutomaton) {}

		const bool hasProhibitions;
		const ProhibitionAutomaton automaton;
	};

	static AutomatonProhibitionsRet findAutomatonProhibitions(const DeterministicTransducerAutomaton & sourceAutomaton) {
		return findAutomatonProhibitionsImpl<AutomatonProhibitionsAutomatonReturnStrategy>(sourceAutomaton);
	}

	static bool automatonHasProhibitions(const DeterministicTransducerAutomaton & sourceAutomaton) {
		return findAutomatonProhibitionsImpl<AutomatonProhibitionsBooleanReturnStrategy>(sourceAutomaton);
	}

private:

	struct AutomatonProhibitionsAutomatonReturnStrategy {

		typedef AutomatonProhibitionsRet result_t;

		typedef ProhibitionAutomatonBuilder builder_t;

		static const bool earlyRet = false;

		static AutomatonProhibitionsRet getResult(bool hasProhibitions, const ProhibitionAutomaton & prohibitionAutomaton) {
			return AutomatonProhibitionsRet(hasProhibitions, prohibitionAutomaton);
		}
	};

	struct AutomatonProhibitionsBooleanReturnStrategy {

		typedef bool result_t;

		static const bool earlyRet = true;

		typedef class Builder {
		public:

			Builder(const DeterministicTransducerAutomaton & sourceAutomaton):
				index(0), states(), statesByIndex() { }

			AddProhibitionAutomatonStateRet addState(const ProhibitionAutomatonState & state) {
				states_t::iterator it = states.find(state);
				bool insert = it == states.end();
				int thisIndex;
				if (!insert)
					thisIndex = it->second;
				else {
					it = states.insert(it, std::make_pair(state, index));
					statesByIndex.push_back(&it->first);
					thisIndex = index;
					index++;
				}
				return AddProhibitionAutomatonStateRet(insert, thisIndex);
			}

			const ProhibitionAutomatonState & getStateByIndex(int index) const {
				return *statesByIndex[index];
			}

			void setTransition(int sourceIndex, int sourceAutomatonOutput, int targetIndex) { }

			int getResult() const { return 42; /* dummy */ }

		private:

			int index;

			typedef std::map<ProhibitionAutomatonState, int> states_t;
			states_t states;

			typedef std::deque<const ProhibitionAutomatonState *> statesbyindex_t;
			statesbyindex_t statesByIndex;

		} builder_t;

		static bool getResult(bool hasProhibitions, int _) {
			return hasProhibitions;
		}
	};

	template <typename ReturnStrategy>
	static typename ReturnStrategy::result_t findAutomatonProhibitionsImpl(
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
				FOREACH(int sourceState, state.sourceStates())
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
};
