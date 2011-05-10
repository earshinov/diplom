#pragma once

#include "../DeterministicTransducerAutomaton.h"
#include "../Utils.h"

#include <stack>
#include <set>
#include <vector>

class AutomatonMaxPreimageCount {
public:

	static const int INF;

private:

	class Calc {
		typedef std::vector<int> counts_t;

		struct CalcState {
			const counts_t counts;
			const int weight;
			const CalcState * const parent;
			const int depth;

			CalcState(const counts_t & counts, int weight) :
				counts(counts), weight(weight), parent(0), depth(0) { }
			CalcState(counts_t && counts, int weight) :
				counts(counts), weight(weight), parent(0), depth(0) { }
			CalcState(const counts_t & counts, int weight, const CalcState & parent) :
				counts(counts), weight(weight), parent(&parent), depth(parent.depth+1) { }
			CalcState(counts_t && counts, int weight, const CalcState & parent) :
				counts(counts), weight(weight), parent(&parent), depth(parent.depth+1) { }

			int compareTo(const CalcState & other) const {
				int w = weight - other.weight;
				if (w) return w;
				return counts < other.counts ? -1 : counts == other.counts ? 0 : 1;
			}

			IMPLEMENT_MEMBER_COMPARE(CalcState)
		};

		const DeterministicTransducerAutomaton & automaton;
		std::vector<std::vector<std::vector<int> > > transitionsByOutput;
		std::set<CalcState> processed;
		std::stack<const CalcState *> q;
		int max;

	public:

		Calc(const DeterministicTransducerAutomaton & automaton) : automaton(automaton),
			transitionsByOutput(automaton.stateSetSize, std::vector<std::vector<int> >(automaton.outputSetSize)) {

			FOREACH_RANGE(int, state, automaton.stateSetSize)
				FOREACH_RANGE(int, input, automaton.inputSetSize)
					auto ret = automaton.transition(state, input);
					transitionsByOutput[state][ret.output].push_back(ret.state);
				FOREACH_END()
			FOREACH_END()

			counts_t counts(automaton.stateSetSize, 1);
			CalcState state(std::move(counts), automaton.stateSetSize);
			const CalcState & inserted = *processed.insert(state).first;
			q.push(&inserted);

			// Намеренно не учитываем вес начального состояния (=automaton.stateSetSize).
			// Мне кажется, число прообразов пустой начальной последовательности логичнее
			// считать равным единице, чем числу состояний автомата.
			max = 1;
		}

		// depth - глубина рекурсии, при достижении которой будет включаться алгоритм определения
		// бесконечной задержки.  Если значение отрицательно, алгоритм будет включаться сразу (что,
		// скорее всего, приведёт к замедлению).
		int calculate(int depth = -1) {
			while (!q.empty()) {
				const CalcState & state = *q.top();
				q.pop();

				if (depth < 0 || state.depth >= depth)
					for (const CalcState * parent = state.parent; parent; parent = parent->parent)
						if (parent->weight < state.weight && eachComponentLe(
							parent->counts.begin(), parent->counts.end(), state.counts.begin()))
						return INF;

				FOREACH_RANGE(int, output, automaton.outputSetSize)
					int weight;
					counts_t counts = step(state.counts, output, &weight);
					add(CalcState(std::move(counts), weight, state));
					max = std::max(max, weight);
				FOREACH_END()
			}
			return max;
		}

	private:

		bool add(const CalcState & state) {
			auto it = processed.find(state);
			if (it != processed.end())
				return false;
			const CalcState & inserted = *processed.insert(it, state);
			q.push(&inserted);
			return true;
		}

		std::vector<int> step(const std::vector<int> & counts, int output, int * weight) const {
			std::vector<int> ret(counts.size());
			*weight = 0;

			FOREACH_RANGE(size_t, state, counts.size())
				int count = counts[state];
				if (count > 0)
					FOREACH(int state2, transitionsByOutput[state][output])
						ret[state2] += count;
						*weight += count;
					FOREACH_END()
			FOREACH_END()

			return ret;
		}

		// каждый компонент первого вектора не больше соответствующего компонента второго
		template <typename Iterator>
		static bool eachComponentLe(Iterator firstBegin, Iterator firstEnd, Iterator secondBegin) {
			for (; firstBegin != firstEnd; ++firstBegin, ++secondBegin)
				if (*firstBegin > *secondBegin)
					return false;
			return true;
		}
	};

public:

	static int findMaxPreimageCount(const DeterministicTransducerAutomaton & automaton) {
		return Calc(automaton).calculate();
	}
};
