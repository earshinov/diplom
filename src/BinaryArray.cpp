#include "BinaryArray.h"


/* static */ BinaryArray BinaryArray::fromInteger(int bitCount, int bits) {
	BinaryArray ret;
	ret.data = std::vector<bool>();
	ret.data.reserve(bitCount);
	for (; bitCount > 0; --bitCount, bits >>= 1 )
		ret.data.push_back(bits & 1);
	return ret;
}
