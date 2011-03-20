#include "../algorithm/Algorithm.h"
#include "../BooleanFunction.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestAutomatonProhibitions)

// digraph{ }
DeterministicTransducerAutomaton createEmpty() {
	return DeterministicTransducerAutomaton(0, 0, 0,
		IntegerFunction(std::vector<int>()), IntegerFunction(std::vector<int>()));
}

// digraph{ 0 }
DeterministicTransducerAutomaton createSingleStateNoTransitions() {
	return DeterministicTransducerAutomaton(0, 0, 1,
		IntegerFunction(std::vector<int>()), IntegerFunction(std::vector<int>()));
}

// digraph{ 0 -> 0 }
DeterministicTransducerAutomaton createSingleStateSingleTransition() {
	return DeterministicTransducerAutomaton(1, 1, 1,
		IntegerFunction(std::vector<int>(1, 0)), IntegerFunction(std::vector<int>(1, 0)));
}

// digraph{ 0->1[label='0:0']; 1->0[label='0:0'] }
DeterministicTransducerAutomaton createTwoStatesCycle() {
	int transition[] = { 1, 0 };
	int output[] = { 0, 0 };
	return DeterministicTransducerAutomaton(1, 1, 2,
		IntegerFunction(std::vector<int>(transition, transition+2)),
		IntegerFunction(std::vector<int>(output, output+2)));
}

// digraph{ 0->1[label='0:0']; 1->0[label='0:1'] }
DeterministicTransducerAutomaton createTwoStatesWithProhibition() {
	int transition[] = { 1, 0 };
	int output[] = { 0, 1 };
	return DeterministicTransducerAutomaton(1, 2, 2,
		IntegerFunction(std::vector<int>(transition, transition+2)),
		IntegerFunction(std::vector<int>(output, output+2)));
}

DeterministicTransducerAutomaton createOriginalExample() {
	int transition[] = { 0, 1, 2, 1, 2, 1, 0, 1 }; // (state, input) -> state
	int output[] = { 0, 0, 1, 0, 1, 0, 0, 0 };
	return DeterministicTransducerAutomaton(2, 2, 4,
		IntegerFunction(std::vector<int>(transition, transition+8)),
		IntegerFunction(std::vector<int>(output, output+8)));
}

DeterministicTransducerAutomaton createProhibitionExample() {
	int transition[] = { 2, 3, 2, 1, 0, 1, 0, 3 }; // (state, input) -> state
	int output[] = { 1, 1, 1, 0, 0, 0, 0, 1 };
	return DeterministicTransducerAutomaton(2, 2, 4,
		IntegerFunction(std::vector<int>(transition, transition+8)),
		IntegerFunction(std::vector<int>(output, output+8)));
}


BOOST_AUTO_TEST_CASE(testEmpty)
{
	DeterministicTransducerAutomaton automaton(createEmpty());
	Algorithm::AutomatonProhibitionsRet ret = Algorithm::findAutomatonProhibitions(automaton);
	BOOST_CHECK(!ret.hasProhibitions);
	BOOST_CHECK(ret.automaton.inputSetSize == 0);
	BOOST_CHECK(ret.automaton.stateSetSize == 1);

	ProhibitionAutomatonState::sourcestateset_t sourceStates;
	BOOST_CHECK(ret.automaton.hasState(sourceStates));
}

BOOST_AUTO_TEST_CASE(testEmptyFast)
{
	BOOST_CHECK(! Algorithm::automatonHasProhibitions(createEmpty()));
}

BOOST_AUTO_TEST_CASE(testSingleStateNoTransitions)
{
	DeterministicTransducerAutomaton automaton(createSingleStateNoTransitions());
	Algorithm::AutomatonProhibitionsRet ret = Algorithm::findAutomatonProhibitions(automaton);
	BOOST_CHECK(!ret.hasProhibitions);
	BOOST_CHECK(ret.automaton.inputSetSize == 0);
	BOOST_CHECK(ret.automaton.stateSetSize == 2);

	ProhibitionAutomatonState::sourcestateset_t sourceStates;
	BOOST_CHECK(ret.automaton.hasState(sourceStates));
	sourceStates.insert(0);
	BOOST_CHECK(ret.automaton.hasState(sourceStates));
}

BOOST_AUTO_TEST_CASE(testSingleStateNoTransitionsFast)
{
	BOOST_CHECK(! Algorithm::automatonHasProhibitions(createSingleStateNoTransitions()));
}

BOOST_AUTO_TEST_CASE(testSingleStateSingleTransition)
{
	DeterministicTransducerAutomaton automaton(createSingleStateSingleTransition());
	Algorithm::AutomatonProhibitionsRet ret = Algorithm::findAutomatonProhibitions(automaton);
	BOOST_CHECK(!ret.hasProhibitions);
	BOOST_CHECK(ret.automaton.inputSetSize == 1);
	BOOST_CHECK(ret.automaton.stateSetSize == 2);

	ProhibitionAutomatonState::sourcestateset_t sourceStates;
	BOOST_CHECK(ret.automaton.hasState(sourceStates));
	sourceStates.insert(0);
	BOOST_CHECK(ret.automaton.hasState(sourceStates));
}

BOOST_AUTO_TEST_CASE(testSingleStateSingleTransitionFast)
{
	BOOST_CHECK(! Algorithm::automatonHasProhibitions(createSingleStateSingleTransition()));
}

BOOST_AUTO_TEST_CASE(testTwoStatesCycle)
{
	DeterministicTransducerAutomaton automaton(createTwoStatesCycle());
	Algorithm::AutomatonProhibitionsRet ret = Algorithm::findAutomatonProhibitions(automaton);
	BOOST_CHECK(!ret.hasProhibitions);
	BOOST_CHECK(ret.automaton.inputSetSize == 1);
	BOOST_CHECK(ret.automaton.stateSetSize == 2);

	ProhibitionAutomatonState::sourcestateset_t sourceStates;
	BOOST_CHECK(ret.automaton.hasState(sourceStates));
	sourceStates.insert(0); sourceStates.insert(1);
	BOOST_CHECK(ret.automaton.hasState(sourceStates));
}

BOOST_AUTO_TEST_CASE(testTwoStatesCycleFast)
{
	BOOST_CHECK(! Algorithm::automatonHasProhibitions(createTwoStatesCycle()));
}

BOOST_AUTO_TEST_CASE(testTwoStatesWithProhibition)
{
	DeterministicTransducerAutomaton automaton(createTwoStatesWithProhibition());
	Algorithm::AutomatonProhibitionsRet ret = Algorithm::findAutomatonProhibitions(automaton);
	BOOST_CHECK(ret.hasProhibitions);
	BOOST_CHECK(ret.automaton.inputSetSize == 2);
	BOOST_CHECK(ret.automaton.stateSetSize == 4);

	ProhibitionAutomatonState::sourcestateset_t sourceStates;
	int q3 = ret.automaton.findState(sourceStates);
	sourceStates.insert(0);
	int q1 = ret.automaton.findState(sourceStates);
	sourceStates.insert(1);
	int q0 = ret.automaton.findState(sourceStates);
	sourceStates.erase(0);
	int q2 = ret.automaton.findState(sourceStates);
	// q0 = {0,1}, q1 = {0}, q2 = {1}, q3 = {}
	BOOST_CHECK(q0 >= 0 && q1 >= 0 && q2 >= 0 && q3 >= 0);

	BOOST_CHECK(ret.automaton.hasTransition(q0, 0, q2));
	BOOST_CHECK(ret.automaton.hasTransition(q0, 1, q1));
	BOOST_CHECK(ret.automaton.hasTransition(q1, 0, q2));
	BOOST_CHECK(ret.automaton.hasTransition(q1, 1, q3));
	BOOST_CHECK(ret.automaton.hasTransition(q2, 0, q3));
	BOOST_CHECK(ret.automaton.hasTransition(q2, 1, q1));
	BOOST_CHECK(ret.automaton.hasTransition(q3, 0, q3));
	BOOST_CHECK(ret.automaton.hasTransition(q3, 1, q3));
}

BOOST_AUTO_TEST_CASE(testTwoStatesWithProhibitionFast)
{
	BOOST_CHECK(Algorithm::automatonHasProhibitions(createTwoStatesWithProhibition()));
}

BOOST_AUTO_TEST_CASE(testOriginalExample)
{
	DeterministicTransducerAutomaton automaton(createOriginalExample());
	Algorithm::AutomatonProhibitionsRet ret = Algorithm::findAutomatonProhibitions(automaton);
	BOOST_CHECK(!ret.hasProhibitions);
	BOOST_CHECK(ret.automaton.inputSetSize == 2);
	BOOST_CHECK(ret.automaton.stateSetSize == 5);

	ProhibitionAutomatonState::sourcestateset_t sourceStates;
	int qNone = ret.automaton.findState(sourceStates);
	sourceStates.insert(1);
	int q1 = ret.automaton.findState(sourceStates);
	sourceStates.insert(0);
	int q01 = ret.automaton.findState(sourceStates);
	sourceStates.clear();
	sourceStates.insert(2);
	int q2 = ret.automaton.findState(sourceStates);
	sourceStates.insert(0);
	sourceStates.insert(1);
	sourceStates.insert(3);
	int qAll = ret.automaton.findState(sourceStates);
	BOOST_CHECK(qNone >= 0 && qAll >= 0 && q1 >= 0 && q01 >= 0 && q2 >= 0);

	BOOST_CHECK(ret.automaton.hasTransition(qAll, 0, q01));
	BOOST_CHECK(ret.automaton.hasTransition(qAll, 1, q2));
	BOOST_CHECK(ret.automaton.hasTransition(q01, 0, q01));
	BOOST_CHECK(ret.automaton.hasTransition(q01, 1, q2));
	BOOST_CHECK(ret.automaton.hasTransition(q2, 0, q1));
	BOOST_CHECK(ret.automaton.hasTransition(q2, 1, q2));
	BOOST_CHECK(ret.automaton.hasTransition(q1, 0, q1));
	BOOST_CHECK(ret.automaton.hasTransition(q1, 1, q2));
	BOOST_CHECK(ret.automaton.hasTransition(qNone, 0, qNone));
	BOOST_CHECK(ret.automaton.hasTransition(qNone, 1, qNone));
}

BOOST_AUTO_TEST_CASE(testOriginalExampleFast)
{
	BOOST_CHECK(! Algorithm::automatonHasProhibitions(createOriginalExample()));
}

BOOST_AUTO_TEST_CASE(testProhibitionExample)
{
	DeterministicTransducerAutomaton automaton(createProhibitionExample());
	Algorithm::AutomatonProhibitionsRet ret = Algorithm::findAutomatonProhibitions(automaton);
	BOOST_CHECK(ret.hasProhibitions);
	BOOST_CHECK(ret.automaton.inputSetSize == 2);
	BOOST_CHECK(ret.automaton.stateSetSize == 8);

	ProhibitionAutomatonState::sourcestateset_t sourceStates;
	int qNone = ret.automaton.findState(sourceStates);
	sourceStates.insert(0);
	int q6 = ret.automaton.findState(sourceStates);
	sourceStates.insert(1);
	int q1 = ret.automaton.findState(sourceStates);
	sourceStates.erase(0);
	int q3 = ret.automaton.findState(sourceStates);
	sourceStates.clear();
	sourceStates.insert(2);
	int q5 = ret.automaton.findState(sourceStates);
	sourceStates.insert(3);
	int q2 = ret.automaton.findState(sourceStates);
	sourceStates.erase(2);
	int q4 = ret.automaton.findState(sourceStates);
	sourceStates.insert(0);
	sourceStates.insert(1);
	sourceStates.insert(2);
	int qAll = ret.automaton.findState(sourceStates);

	BOOST_CHECK(qNone >= 0 && qAll >= 0 &&
		q1 >= 0 && q2 >= 0 && q3 >= 0 &&
		q4 >= 0 && q5 >= 0 && q6 >= 0);

	BOOST_CHECK(ret.automaton.hasTransition(qAll, 0, q1));
	BOOST_CHECK(ret.automaton.hasTransition(qAll, 1, q2));
	BOOST_CHECK(ret.automaton.hasTransition(q1, 0, q3));
	BOOST_CHECK(ret.automaton.hasTransition(q1, 1, q2));
	BOOST_CHECK(ret.automaton.hasTransition(q2, 0, q1));
	BOOST_CHECK(ret.automaton.hasTransition(q2, 1, q4));
	BOOST_CHECK(ret.automaton.hasTransition(q3, 0, q3));
	BOOST_CHECK(ret.automaton.hasTransition(q3, 1, q5));
	BOOST_CHECK(ret.automaton.hasTransition(q5, 0, q1));
	BOOST_CHECK(ret.automaton.hasTransition(q5, 1, qNone));
	BOOST_CHECK(ret.automaton.hasTransition(q4, 0, q6));
	BOOST_CHECK(ret.automaton.hasTransition(q4, 1, q4));
	BOOST_CHECK(ret.automaton.hasTransition(q6, 0, qNone));
	BOOST_CHECK(ret.automaton.hasTransition(q6, 1, q2));
	BOOST_CHECK(ret.automaton.hasTransition(qNone, 0, qNone));
	BOOST_CHECK(ret.automaton.hasTransition(qNone, 1, qNone));
}

BOOST_AUTO_TEST_CASE(testProhibitionExampleFast)
{
	BOOST_CHECK(Algorithm::automatonHasProhibitions(createProhibitionExample()));
}

BOOST_AUTO_TEST_SUITE_END()
