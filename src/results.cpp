#include "algorithm/AutomatonDelay.h"
#include "algorithm/AutomatonMaxPreimageCount.h"
#include "algorithm/AutomatonProhibitions.h"
#include "AutomatonWithInputOutputMemory.h"
#include "results.h"

#include <cassert>
#include <iostream>

static void writeBits(std::ostream & o, int bitCount, int bits) {
	// младший бит выводим первым
	for (; bitCount > 0; --bitCount, bits >>= 1)
		std::cout << (bits % 2 ? '1' : '0');
}

std::string resultsHeader() {
	return "#L,M,func,hasProhibitions,delay,hasInformationLoss,maxPreimageCount";
}

void generateOneResult(int l, int m, int bits) {
	int argumentCount = l + m + 1;
	int bitCount = 1 << argumentCount;
	BooleanFunction func(argumentCount, BinaryArray::fromInteger(bitCount, bits));
	AutomatonWithInputOutputMemory aiom(l, m, func);
	DeterministicTransducerAutomaton automaton = aiom.getAutomaton();

	bool hasProhibitions = AutomatonProhibitions::automatonHasProhibitions(automaton);
	auto delayRet = AutomatonDelay::findAutomatonDelay(automaton);
	int preimageCount = AutomatonMaxPreimageCount::findMaxPreimageCount(automaton);

	// размеры входной и выходной памяти
	std::cout << l << ',' << m << ',';
	// столбец функции
	writeBits(std::cout, bitCount, bits);
	std::cout << ',';
	// наличие запретов
	std::cout << (hasProhibitions ? "Есть" : "Нет");
	std::cout << ',';
	// задержка
	delayRet.automatonDelay == AUTOMATON_DELAY_INF ? std::cout << "Бескон" : std::cout << delayRet.automatonDelay;
	std::cout << ',';
	// наличие потерь информации
	std::cout << (delayRet.hasInformationLoss ? "Есть" : "Нет");
	std::cout << ',';
	// максимальное число прообразов
	preimageCount == AutomatonMaxPreimageCount::PROBABLY_INF ? std::cout << "Бескон" : std::cout << preimageCount;
	std::cout << '\n';
}

void generateResults(int lStart, int lEnd, int mStart, int mEnd, int bits /* = 0 */) {
	for (int l = lStart; l <= lEnd; ++l) {
		for (int m = mStart; m <= mEnd; ++m) {
			int max = 1 << (1 << (l + m + 1));
			for (; bits < max; ++bits)
				generateOneResult(l, m, bits);
			bits = 0;
		}
	}
}
