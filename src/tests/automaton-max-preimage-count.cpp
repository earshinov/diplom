#include "automaton-examples.h"
#include "../algorithm/AutomatonMaxPreimageCount.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestAutomatonMaxPreimageCount)

BOOST_AUTO_TEST_CASE(testEmpty)
{
	int ret = AutomatonMaxPreimageCount::findMaxPreimageCount(createEmpty());
	BOOST_CHECK_EQUAL(ret, 1); // 1 - число прообразов пустой вых. последовательности
}

BOOST_AUTO_TEST_CASE(testSingleStateNoTransitions)
{
	int ret = AutomatonMaxPreimageCount::findMaxPreimageCount(createSingleStateNoTransitions());
	BOOST_CHECK_EQUAL(ret, 1); // 1 - число прообразов пустой вых. последовательности
}

BOOST_AUTO_TEST_CASE(testSingleStateSingleTransition)
{
	int ret = AutomatonMaxPreimageCount::findMaxPreimageCount(createSingleStateSingleTransition());
	BOOST_CHECK_EQUAL(ret, 1);
}

BOOST_AUTO_TEST_CASE(testTwoStatesCycle)
{
	int ret = AutomatonMaxPreimageCount::findMaxPreimageCount(createTwoStatesCycle());
	BOOST_CHECK_EQUAL(ret, 2);
}

BOOST_AUTO_TEST_CASE(testTwoStatesWithProhibition)
{
	int ret = AutomatonMaxPreimageCount::findMaxPreimageCount(createTwoStatesWithProhibition());
	BOOST_CHECK_EQUAL(ret, 1);
}

BOOST_AUTO_TEST_CASE(testOriginalExample)
{
	int ret = AutomatonMaxPreimageCount::findMaxPreimageCount(createOriginalExample());
	BOOST_CHECK_EQUAL(ret, AutomatonMaxPreimageCount::PROBABLY_INF);
}

BOOST_AUTO_TEST_CASE(testProhibitionExample)
{
	int ret = AutomatonMaxPreimageCount::findMaxPreimageCount(createProhibitionExample());
	BOOST_CHECK_EQUAL(ret, AutomatonMaxPreimageCount::PROBABLY_INF);
}

BOOST_AUTO_TEST_CASE(testFiniteDelayExample)
{
	int ret = AutomatonMaxPreimageCount::findMaxPreimageCount(createFiniteDelayExample());
	BOOST_CHECK_EQUAL(ret, 4);
}

BOOST_AUTO_TEST_CASE(testFiniteMaxPreimageCount)
{
	int ret = AutomatonMaxPreimageCount::findMaxPreimageCount(createFiniteMaxPreimageCountExample());
	BOOST_CHECK_EQUAL(ret, 8);
}

BOOST_AUTO_TEST_SUITE_END()
