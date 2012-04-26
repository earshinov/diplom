#include "algorithm/AutomatonDelay.h"
#include "algorithm/AutomatonMaxPreimageCount.h"
#include "algorithm/AutomatonProhibitions.h"
#include "AutomatonWithInputOutputMemory.h"
#include "results.h"
#include "unicode.h"

#include <cassert>
#include <iostream>
#include <sstream>

static void writeBits(std::ostream & out, int bitCount, int bits) {
	// младший бит выводим первым
	for (; bitCount > 0; --bitCount, bits >>= 1)
		out << (bits % 2 ? '1' : '0');
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

	std::stringstream ss;
	// размеры входной и выходной памяти
	ss << l << ',' << m << ',';
	// столбец функции
	writeBits(ss, bitCount, bits);
	ss << ',';
	// наличие запретов
	ss << (hasProhibitions ? "Есть" : "Нет");
	ss << ',';
	// задержка
	delayRet.automatonDelay == AUTOMATON_DELAY_INF ? ss << "Бескон" : ss << delayRet.automatonDelay;
	ss << ',';
	// наличие потерь информации
	ss << (delayRet.hasInformationLoss ? "Есть" : "Нет");
	ss << ',';
	// максимальное число прообразов
	preimageCount == AutomatonMaxPreimageCount::PROBABLY_INF ? ss << "Бескон" : ss << preimageCount;
	std::cout << U(ss.str()) << std::endl;
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
