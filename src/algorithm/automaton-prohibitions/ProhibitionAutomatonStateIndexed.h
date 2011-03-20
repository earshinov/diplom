#pragma once

#include "ProhibitionAutomatonState.h"
#include "../../Utils.h"

struct ProhibitionAutomatonStateIndexed {

	ProhibitionAutomatonStateIndexed(int index, const ProhibitionAutomatonState & state) :
	    index(index), state(state) {}

	int compareTo(const ProhibitionAutomatonStateIndexed & other) const {
		return state.compareTo(other.state);
	}

	const int index;
	const ProhibitionAutomatonState state;
};

IMPLEMENT_COMPARE(ProhibitionAutomatonStateIndexed)
