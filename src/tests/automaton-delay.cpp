#include "automaton-examples.h"
#include "../algorithm/AutomatonDelay.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestAutomatonDelay)

BOOST_AUTO_TEST_CASE(testEmpty)
{
	auto ret = AutomatonDelay::findAutomatonDelay(createEmpty());
	BOOST_CHECK_EQUAL(ret.automatonDelay, 0);
	BOOST_CHECK(!ret.hasInformationLoss);
}

BOOST_AUTO_TEST_CASE(testSingleStateNoTransitions)
{
	auto ret = AutomatonDelay::findAutomatonDelay(createSingleStateNoTransitions());
	BOOST_CHECK_EQUAL(ret.delayByState[0], 0);
	BOOST_CHECK_EQUAL(ret.automatonDelay, 0);
	BOOST_CHECK(!ret.hasInformationLoss);
}

BOOST_AUTO_TEST_CASE(testSingleStateSingleTransition)
{
	auto ret = AutomatonDelay::findAutomatonDelay(createSingleStateSingleTransition());
	BOOST_CHECK_EQUAL(ret.delayByState[0], 0);
	BOOST_CHECK_EQUAL(ret.automatonDelay, 0);
	BOOST_CHECK(!ret.hasInformationLoss);
}

BOOST_AUTO_TEST_CASE(testTwoStatesCycle)
{
	auto ret = AutomatonDelay::findAutomatonDelay(createTwoStatesCycle());
	BOOST_CHECK_EQUAL(ret.delayByState[0], 0);
	BOOST_CHECK_EQUAL(ret.delayByState[1], 0);
	BOOST_CHECK_EQUAL(ret.automatonDelay, 0);
	BOOST_CHECK(!ret.hasInformationLoss);
}

BOOST_AUTO_TEST_CASE(testTwoStatesWithProhibition)
{
	auto ret = AutomatonDelay::findAutomatonDelay(createTwoStatesWithProhibition());
	BOOST_CHECK_EQUAL(ret.delayByState[0], 0);
	BOOST_CHECK_EQUAL(ret.delayByState[1], 0);
	BOOST_CHECK_EQUAL(ret.automatonDelay, 0);
	BOOST_CHECK(!ret.hasInformationLoss);
}

BOOST_AUTO_TEST_CASE(testOriginalExample)
{
	auto ret = AutomatonDelay::findAutomatonDelay(createOriginalExample());
	BOOST_CHECK_EQUAL(ret.delayByState[0], AUTOMATON_DELAY_INF);
	BOOST_CHECK_EQUAL(ret.delayByState[1], 0);
	BOOST_CHECK_EQUAL(ret.delayByState[2], 0);
	BOOST_CHECK_EQUAL(ret.delayByState[3], AUTOMATON_DELAY_INF);
	BOOST_CHECK_EQUAL(ret.automatonDelay, AUTOMATON_DELAY_INF);
	BOOST_CHECK(ret.hasInformationLoss);
}

BOOST_AUTO_TEST_CASE(testProhibitionExample)
{
	auto ret = AutomatonDelay::findAutomatonDelay(createProhibitionExample());
	BOOST_CHECK_EQUAL(ret.delayByState[0], AUTOMATON_DELAY_INF);
	BOOST_CHECK_EQUAL(ret.delayByState[1], 0);
	BOOST_CHECK_EQUAL(ret.delayByState[2], AUTOMATON_DELAY_INF);
	BOOST_CHECK_EQUAL(ret.delayByState[3], 0);
	BOOST_CHECK_EQUAL(ret.automatonDelay, AUTOMATON_DELAY_INF);
	BOOST_CHECK(ret.hasInformationLoss);
}

BOOST_AUTO_TEST_CASE(testFiniteDelayExample)
{
	auto ret = AutomatonDelay::findAutomatonDelay(createFiniteDelayExample());
	BOOST_CHECK_EQUAL(ret.delayByState[0], 1);
	BOOST_CHECK_EQUAL(ret.delayByState[1], 2);
	BOOST_CHECK_EQUAL(ret.delayByState[2], 2);
	BOOST_CHECK_EQUAL(ret.delayByState[3], 0);
	BOOST_CHECK_EQUAL(ret.delayByState[4], 0);
	BOOST_CHECK_EQUAL(ret.automatonDelay, 2);
	BOOST_CHECK(!ret.hasInformationLoss);
}

BOOST_AUTO_TEST_SUITE_END()
