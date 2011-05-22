#pragma once

#include "../DeterministicTransducerAutomaton.h"

#include <deque>


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


class AutomatonDelay {
public:

	static AutomatonDelayRet findAutomatonDelay(const DeterministicTransducerAutomaton & sourceAutomaton);
};
