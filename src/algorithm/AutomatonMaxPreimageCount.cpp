#include "AutomatonMaxPreimageCount.h"
#include "../Utils.h"

#include <stack>
#include <set>
#include <vector>


/* static */ const int AutomatonMaxPreimageCount::PROBABLY_INF = -1;


class OverflowException : public std::exception { };

class Calc {

	typedef std::vector<int> counts_t;

	const DeterministicTransducerAutomaton & automaton;
	std::vector<std::vector<std::vector<int> > > transitionsByOutput;
	std::set<counts_t> processed;
	std::stack<counts_t> q;
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
		// Намеренно не учитываем вес начального состояния (=automaton.stateSetSize).
		// Мне кажется, число прообразов пустой начальной последовательности логичнее
		// считать равным единице, чем числу состояний автомата.
		max = 1;
		add(counts);
	}

	int calculate(int iterationLimit = -1) {
		try {
			counts_t counts;
			while(!q.empty() && (iterationLimit < 0 || --iterationLimit >= 0)) {
				std::swap(counts, q.top());
				q.pop();

				FOREACH_RANGE(int, output, automaton.outputSetSize)
					int sum;
					add(step(counts, output, &sum));
					max = std::max(max, sum);
				FOREACH_END()
			}
			return q.empty() ? max : AutomatonMaxPreimageCount::PROBABLY_INF;
		}
		catch(const OverflowException &) {
			return AutomatonMaxPreimageCount::PROBABLY_INF;
		}
	}

private:

	bool add(const counts_t & counts) {
		if (processed.find(counts) != processed.end())
			return false;
		processed.insert(counts);
		q.push(counts);
		return true;
	}

	std::vector<int> step(const std::vector<int> & counts, int output, int * sum) const {
		std::vector<int> ret(counts.size());
		*sum = 0;

		FOREACH_RANGE(size_t, state, counts.size())
			int count = counts[state];
			if (count > 0)
				FOREACH(int, state2, transitionsByOutput[state][output])
					ret[state2] += count;
					if (*sum + count < *sum)
						throw OverflowException();
					*sum += count;
				FOREACH_END()
		FOREACH_END()

		return ret;
	}
};


/* static */ int AutomatonMaxPreimageCount::findMaxPreimageCount(
	const DeterministicTransducerAutomaton & automaton) {

	int iterationLimit = 16000;
	return Calc(automaton).calculate(iterationLimit);
}
