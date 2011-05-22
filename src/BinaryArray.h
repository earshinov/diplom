#pragma once

#include <cstddef> // size_t
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

	static BinaryArray fromInteger(int bitCount, int bits);

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

	BinaryArray() { }

	std::vector<bool> data;
};
