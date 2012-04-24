#include "../AutomatonWithInputOutputMemory.h"

#include <boost/test/unit_test.hpp>

typedef AutomatonWithInputOutputMemory AIOM;
typedef DeterministicTransducerAutomaton Automaton;

BOOST_AUTO_TEST_SUITE(TestAiomAutomaton)

BooleanFunction getFunc(int nArguments, int valuesAsInteger) {
	return BooleanFunction(nArguments, BinaryArray::fromInteger(1 << nArguments, valuesAsInteger));
}

Automaton getAutomaton(int inputMemorySize, int outputMemorySize, int funcValuesAsInteger) {
	BooleanFunction func = getFunc(1 + inputMemorySize + outputMemorySize, funcValuesAsInteger);
	return AIOM(inputMemorySize, outputMemorySize, func).getAutomaton();
}

#define CHECK_TRANSITION(output0, state0, output1, state1) \
	BOOST_CHECK_EQUAL(automaton.transition(output0, state0), Automaton::Transition(output1, state1))

BOOST_AUTO_TEST_CASE(testEmpty)
{
	Automaton automaton = getAutomaton(0, 0, 0x0);

	// всегда для автомата, построенного по AIOM
	// в дальнейшем проверять это не будем
	BOOST_CHECK_EQUAL(automaton.inputSetSize, 2);
	BOOST_CHECK_EQUAL(automaton.outputSetSize, 2);

	// пустой автомат имеет единственное состояние
	BOOST_CHECK_EQUAL(automaton.stateSetSize, 1);
	CHECK_TRANSITION(0, 0, 0, 0);
	CHECK_TRANSITION(0, 1, 0, 0);
}

BOOST_AUTO_TEST_CASE(testOriginalExample)
{
	Automaton automaton = getAutomaton(1, 1, 0x14);
	BOOST_CHECK_EQUAL(automaton.stateSetSize, 4);

	// Как понять, с какими аргументами вызывается функция:
	// приписываем к состоянию входной символ СПРАВА (младшим разрядом).
	// Интерпретируя полученную последовательность как целое число,
	// получаем индекс в столбце функции.  В двоичной последовательности,
	// описывающей этот столбец, индекс отсчитывается СПРАВА.
	//
	// Таким образом, последний столбец в столбце проверок CHECK_TRANSITION
	// *совпадает* со столбцом функции и *обратен* двоичной последовательности,
	// использованной при задании функции.
	//
	// Конкретно для этого автомата (и для любого с L = M = 1)
	// действует также простая закономерность: все варианты имеют вид
	// CHECK_TRANSITION(0b__, X, 0bYX, Y)

	CHECK_TRANSITION(0x0, 0, 0x0, 0);
	CHECK_TRANSITION(0x0, 1, 0x1, 0);
	CHECK_TRANSITION(0x1, 0, 0x2, 1);
	CHECK_TRANSITION(0x1, 1, 0x1, 0);
	CHECK_TRANSITION(0x2, 0, 0x2, 1);
	CHECK_TRANSITION(0x2, 1, 0x1, 0);
	CHECK_TRANSITION(0x3, 0, 0x0, 0);
	CHECK_TRANSITION(0x3, 1, 0x1, 0);
}

BOOST_AUTO_TEST_CASE(testSingleInputMemory)
{
	Automaton automaton = getAutomaton(1, 0, 0xD); // импликация
	BOOST_CHECK_EQUAL(automaton.stateSetSize, 2);

	CHECK_TRANSITION(0x0, 0, 0x0, 1);
	CHECK_TRANSITION(0x0, 1, 0x1, 0);
	CHECK_TRANSITION(0x1, 0, 0x0, 1);
	CHECK_TRANSITION(0x1, 1, 0x1, 1);
}

BOOST_AUTO_TEST_CASE(testSingleOutputMemory)
{
	Automaton automaton = getAutomaton(0, 1, 0xD); // импликация
	BOOST_CHECK_EQUAL(automaton.stateSetSize, 2);

	CHECK_TRANSITION(0x0, 0, 0x1, 1);
	CHECK_TRANSITION(0x0, 1, 0x0, 0);
	CHECK_TRANSITION(0x1, 0, 0x1, 1);
	CHECK_TRANSITION(0x1, 1, 0x1, 1);
}

BOOST_AUTO_TEST_SUITE_END()
