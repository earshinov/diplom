#pragma once

#include "../../Utils.h"

#include <set>

class ProhibitionAutomatonState {
public:

	typedef std::set<int> sourcestateset_t;

public:

	/* implicit */ ProhibitionAutomatonState(std::initializer_list<int> sourceStates) :
		sourceStates_(sourceStates) { }
	/* implicit */ ProhibitionAutomatonState(const sourcestateset_t & sourceStates) :
		sourceStates_(sourceStates) { }
	/* implicit */ ProhibitionAutomatonState(sourcestateset_t && sourceStates) :
		sourceStates_(std::move(sourceStates)) { }

	bool empty() const {
		return weight() == 0;
	}

	int weight() const {
		return sourceStates_.size();
	}

	const sourcestateset_t & sourceStates() const {
		return sourceStates_;
	}

	int compareTo(const ProhibitionAutomatonState & other) const {
		return sourceStates_ < other.sourceStates_ ? -1 :
			sourceStates_ == other.sourceStates_ ? 0 : 1;
	}

private:

	sourcestateset_t sourceStates_;
};

IMPLEMENT_COMPARE(ProhibitionAutomatonState)
