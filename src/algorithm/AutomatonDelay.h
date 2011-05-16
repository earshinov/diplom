#pragma once

#include "../DeterministicTransducerAutomaton.h"
#include "../IndexedSet.h"
#include "../Utils.h"

#include <deque>
#include <set>


// Потенциальные возможности оптимизации алгоритма определения задержки автомата:
//
// 1. На самом деле алгоритм определения задержек можно кардинально упростить.  Этапы
// построения автомата и его обхода можно легко объединить, превратив алгоритм в простейшую
// рекурсию с мемоизацией.  Автомат можно будет извести совсем.
// Конечно, такой способ не пройдёт, если явный автомат всё-таки нужен.
//
// 2. Пары состояний исходного автомата естественно рассматривать неупорядоченными.  Это
// сократит объём работы до двух раз.
//
// 3. Когда получаем пару состояний исходного автомата, в которой первый и второй элемент совпадают,
// можем сразу установить бесконечное значение задержки вместо того, чтобы обрабатывать эту пару
// до тех пор, пока не получим явный цикл в автомате для определения задержки.  А такой цикл мы
// получим всегда: он будет соответствовать циклу в исходном автомате, а из любого состояния
// детерминированного автомата с непустым множеством входов можно провести бесконечный путь (то есть,
// вследствие конечности автомата, содержащий цикл).  Множество входов у нас непустое, иначе мы в
// алгоритме определения задержки не дошли бы до *пар* состояний исходного автомата.


extern const int AUTOMATON_DELAY_INF;
extern const int AUTOMATON_DELAY_UNKNOWN;


struct AutomatonDelayRet {

	std::deque<int> delayByState;
	int automatonDelay;
	bool hasInformationLoss;

	AutomatonDelayRet(const std::deque<int> & delayByState, int automatonDelay, bool hasInformationLoss) :
		delayByState(delayByState), automatonDelay(automatonDelay), hasInformationLoss(hasInformationLoss) { }
};


class AutomatonDelayAutomatonState {
public:

	AutomatonDelayAutomatonState(int sourceState):
		sourceState(sourceState), sourceState2(-1), children(),
		delay(AUTOMATON_DELAY_UNKNOWN) { }

	AutomatonDelayAutomatonState(int state1, int state2):
		sourceState(state1), sourceState2(state2), children(),
		delay(AUTOMATON_DELAY_UNKNOWN) { }

	bool hasSourceState2() const {
		return sourceState2 != -1;
	}

	int compareTo(const AutomatonDelayAutomatonState & other) const {
		int diff = sourceState - other.sourceState;
		if (diff != 0)
			return diff;
		return sourceState2 - other.sourceState2;
	}

	void addChild(int childIndex) {
		children.insert(childIndex);
	}

	const int sourceState;
	const int sourceState2; // may be -1

	std::set<int> children;
	int delay;
};

IMPLEMENT_COMPARE(AutomatonDelayAutomatonState)


class AutomatonDelayCalculation {
public:

	AutomatonDelayCalculation(int nSourceStates):
		states(), startingStateIndexes(), indexesToProcess(),
		delayByState(nSourceStates, AUTOMATON_DELAY_UNKNOWN),
		hasInformationLoss(false) { }

	int addStartingState(const AutomatonDelayAutomatonState & state) {
		auto ret = states.insert(state);
		assert(ret.inserted);
		startingStateIndexes.push_back(ret.index);
		return ret.index;
	}

	void addStateToProcess(const AutomatonDelayAutomatonState & state, int parentIndex) {
		auto ret = states.insert(state);
		getStateByIndex(parentIndex).addChild(ret.index);
		if (ret.inserted) {
			indexesToProcess.push_back(ret.index);
			/* необходимое и достаточное условие наличия потерь информации - наличие пары (q,q) */
			if (state.sourceState == state.sourceState2)
				hasInformationLoss = true;
		}
	}

	bool hasStatesToProcess() const {
		return !indexesToProcess.empty();
	}

	int getStateToProcess() {
		int index = indexesToProcess.front();
		indexesToProcess.pop_front();
		return index;
	}

	AutomatonDelayAutomatonState & getStateByIndex(int index) {
		return states.get(index);
	}

	void setStateDelay(int sourceState, int delay) {
		delayByState[sourceState] = delay;
	}

	AutomatonDelayRet calculateResult() {
		FOREACH(int, index, startingStateIndexes)
			AutomatonDelayAutomatonState & state = getStateByIndex(index);
			calculateStateDelay(state);
			delayByState[state.sourceState] = state.delay;
		FOREACH_END()
		return AutomatonDelayRet(delayByState, calculateAutomatonDelay(), hasInformationLoss);
	}

private:

	void calculateStateDelay(AutomatonDelayAutomatonState & thisState) {
		// для отлова циклов
		thisState.delay = AUTOMATON_DELAY_INF;
		// реальное значение задержки, которое затем запишем в thisState
		int delay = 0;
		FOREACH(int, index, thisState.children)
			AutomatonDelayAutomatonState & state = getStateByIndex(index);
			if (state.delay == AUTOMATON_DELAY_UNKNOWN)
				calculateStateDelay(state);
			if (state.delay == AUTOMATON_DELAY_INF) {
				// оставляем значение thisState.delay = AUTOMATON_DELAY_INF;
				return;
			}
			delay = std::max(delay, state.delay + 1);
		FOREACH_END()
		thisState.delay = delay;
	}

	int calculateAutomatonDelay() const {
		int max = 0; // пусть пустой автомат имеет нулевую задержку
		FOREACH(int, value, delayByState)
			if (value == AUTOMATON_DELAY_INF || value == AUTOMATON_DELAY_UNKNOWN) {
				max = value;
				break;
			}
			max = std::max(max, value);
		FOREACH_END()
		return max;
	}

private:

	IndexedSet<AutomatonDelayAutomatonState> states;
	std::deque<int> startingStateIndexes;
	std::deque<int> indexesToProcess;
	std::deque<int> delayByState; // integer >= 0 | DELAY_*
	bool hasInformationLoss;
};


class AutomatonDelay {
public:

	static AutomatonDelayRet findAutomatonDelay(const DeterministicTransducerAutomaton & sourceAutomaton) {
		AutomatonDelayCalculation calc(sourceAutomaton.stateSetSize);

		FOREACH_RANGE(int, sourceState, sourceAutomaton.stateSetSize)
			std::deque<std::deque<int> > targetStatesByOutput(sourceAutomaton.outputSetSize);

			FOREACH_RANGE(int, input, sourceAutomaton.inputSetSize)
				auto ret = sourceAutomaton.transition(sourceState, input);
				targetStatesByOutput[ret.output].push_back(ret.state);
			FOREACH_END()

			bool hasChildren = false;
			FOREACH_RANGE(int, output, sourceAutomaton.outputSetSize)
				const std::deque<int> & targetStates = targetStatesByOutput[output];
				if (targetStates.size() >= 2) {
					hasChildren = true;
					int parentIndex = calc.addStartingState(AutomatonDelayAutomatonState(sourceState));
					foreach_2_tuples(targetStates.begin(), targetStates.end(), [&](int state1, int state2) {
						calc.addStateToProcess(AutomatonDelayAutomatonState(state1, state2), parentIndex);
					} );
				}
			FOREACH_END()

			if (!hasChildren)
				calc.setStateDelay(sourceState, 0);
		FOREACH_END()

		while (calc.hasStatesToProcess()) {
			int index = calc.getStateToProcess();
			const AutomatonDelayAutomatonState & state = calc.getStateByIndex(index);
			assert(state.hasSourceState2());

			FOREACH_RANGE(int, input, sourceAutomaton.inputSetSize)
				auto first = sourceAutomaton.transition(state.sourceState, input);
				FOREACH_RANGE(int, input2, sourceAutomaton.inputSetSize)
					auto second = sourceAutomaton.transition(state.sourceState2, input2);
					if (first.output == second.output)
						calc.addStateToProcess(AutomatonDelayAutomatonState(first.state, second.state), index);
				FOREACH_END()
			FOREACH_END()
		}

		return calc.calculateResult();
	}
};
