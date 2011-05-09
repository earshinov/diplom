#pragma once

#include <cstddef>
#include <vector>

class IntegerFunction {
public:

	typedef std::vector<int> values_t;

public:

	IntegerFunction(const values_t values): values(values) {}

	size_t size() const {
		return values.size();
	}

	int evaluate(int argumentsAsInteger) const {
		return values[argumentsAsInteger];
	}

private:

	const std::vector<int> values;
};
