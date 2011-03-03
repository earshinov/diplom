#pragma once

#include "BinaryArray.h"

#include <cassert>

class BooleanFunction {
public:

	BooleanFunction(int nArguments, const BinaryArray values):
		nArguments(nArguments), values(values)
	{
		assert(values.size() == 1u << nArguments);
	}

	bool evaluate(int argumentsAsInteger) const {
		return values[argumentsAsInteger];
	}

private:

	const int nArguments;
	const BinaryArray values;
};
