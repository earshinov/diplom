#pragma once

#include "BooleanFunction.h"
#include "DeterministicTransducerAutomaton.h"

#include <vector>

class AutomatonWithInputOutputMemory {
public:

	AutomatonWithInputOutputMemory(int inputMemorySize, int outputMemorySize, BooleanFunction func):
		inputMemorySize(inputMemorySize), outputMemorySize(outputMemorySize), func(func) {}

	// Альтернативный подход - не конструировать автомат в явном виде, а преобразовать
	// DeterministicTransducerAutomaton в интерфейс и реализовать его в этом классе
	// без использования дополнительной памяти.  Такой подход, скорее всего, более эффективен, но
	// труднее в кодировании, так что, пока не проведены тесты производительности,
	// это преждевременная оптимизация
	DeterministicTransducerAutomaton getAutomaton() const {
		int sz = 1 << (inputMemorySize + outputMemorySize);
		int inputMemoryMask = (1 << inputMemorySize) - 1;
		std::vector<int> transitionFunction(sz * 2);
		std::vector<int> outputFunction(sz * 2);

		for (int state = 0; state < sz; ++state) {
			int input0 = state << 1, input1 = input0 + 1;
			bool output0, output1;
			int state0 = automatonStep(input0, inputMemoryMask, sz, &output0);
			int state1 = automatonStep(input1, inputMemoryMask, sz, &output1);

			transitionFunction[input0] = state0;
			transitionFunction[input1] = state1;
			outputFunction[input0] = output0;
			outputFunction[input1] = output1;
		}

		return DeterministicTransducerAutomaton(2, 2, sz,
			IntegerFunction(std::move(transitionFunction)),
			IntegerFunction(std::move(outputFunction)));
	}

private:

	int automatonStep(int state, int inputMemoryMask, int sz, bool * output) const {
		*output = func.evaluate(state);
		return (state & inputMemoryMask) | ((((state >> 1) + (*output * sz)) >> 1) & ~inputMemoryMask);
	}

	const int inputMemorySize;
	const int outputMemorySize;
	const BooleanFunction func;
};
