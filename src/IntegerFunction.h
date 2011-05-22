#pragma once

#include <cstddef> // size_t
#include <vector>

class IntegerFunction {
public:

	typedef std::vector<int> values_t;

public:

	IntegerFunction(const values_t & values) : values(values) {}
	IntegerFunction(const values_t && values) : values(values) {}

	size_t size() const {
		return values.size();
	}

	int evaluate(int argumentsAsInteger) const {
		return values[argumentsAsInteger];
	}

private:

	const values_t values;
};
