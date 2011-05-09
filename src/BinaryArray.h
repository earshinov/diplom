#pragma once

#include <cstddef>
#include <vector>

class BinaryArray {
private:

	class Reference {
	public:

		Reference(BinaryArray & parent, int index) :
			parent(&parent), index(index) {}

		operator bool() const {
			return parent->data[index];
		}

		Reference & operator=(bool value) {
			parent->data[index] = value;
			return *this;
		}

	private:

		BinaryArray * parent;
		const int index;
	};

public:

	static BinaryArray fromInteger(int bits, int integer) {
		BinaryArray ret;
		ret.data = std::vector<bool>();
		ret.data.reserve(bits);
		for (; bits > 0; integer >>= 1, --bits)
			ret.data.push_back(integer & 1);
		return ret;
	}

	size_t size() const {
		return data.size();
	}

	Reference operator[](int index) {
		return Reference(*this, index);
	}

	bool operator[](int index) const {
		return data[index];
	}

private:

	std::vector<bool> data;
};
