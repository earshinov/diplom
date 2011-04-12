#include "automaton-examples.h"
#include "../algorithm/AutomatonDelay.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestAutomatonDelay)

BOOST_AUTO_TEST_CASE(testOriginalExample)
{
	auto ret = AutomatonDelay::findAutomatonDelay(createOriginalExample());
	BOOST_CHECK_EQUAL(ret.delayByState[0], AUTOMATON_DELAY_INF);
	BOOST_CHECK_EQUAL(ret.delayByState[1], 0);
	BOOST_CHECK_EQUAL(ret.delayByState[2], 0);
	BOOST_CHECK_EQUAL(ret.delayByState[3], AUTOMATON_DELAY_INF);
	BOOST_CHECK_EQUAL(ret.automatonDelay, AUTOMATON_DELAY_INF);
}

BOOST_AUTO_TEST_SUITE_END()
