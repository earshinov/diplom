#pragma once

#include "Utils.h"

#include <cstddef> // size_t
#include <deque>
#include <set>

template <typename T>
class IndexedSet {
public:

	struct InsertRet {
		int index;
		T & object;
		bool inserted;

		InsertRet(int index, T & object, bool inserted) :
			index(index), object(object), inserted(inserted) { }
	};

	InsertRet insert(T object) {
		auto ret = set.insert(Indexed(std::move(object), byIndex.size()));
		if (ret.second)
			byIndex.push_back(&ret.first->object);
		return InsertRet(ret.first->index, ret.first->object, ret.second);
	}

	size_t size() const {
		return byIndex.size();
	}

	T & get(int index) {
		return *byIndex[index];
	}

	const T & get(int index) const {
		return *byIndex[index];
	}

	int find(const T & object) const {
		auto it = set.find(Indexed(object, -1));
		return it == set.end() ? -1 : it->index;
	}

	int find(T && object) const {
		auto it = set.find(Indexed(std::move(object), -1));
		return it == set.end() ? -1 : it->index;
	}

private:

	struct Indexed {

		// Используем mutable, потому что экземпляры класса Indexed помещаются в set, который
		// возвращает назад лишь константные ссылки.  Мы же возвращаем в пользовательский код
		// ссылки, которые можно использовать для модификации; НО при написании пользовательского кода
		// нужно иметь в виду, что изменение уже после вставки в set полей класса T, используемых
		// при сравнении экземпляров класса T, повлечёт непредсказуемое поведение set!
		mutable T object;

		int index;

		Indexed(const T & object, int index) : object(object), index(index) { }
		Indexed(T && object, int index) : object(std::move(object)), index(index) { }

		int compareTo(const Indexed & other) const {
			return object < other.object ? -1 : other.object < object ? 1 : 0;
		}

		// Не можем вынести определение операторов сравнения на верхний уровень, потому что
		// компилятор (во всяком случае, GCC) их просто не обнаруживает.  Видимо, из-за шаблонности
		// структуры для того, чтобы такие операторы нормально обнаруживались, их нужно явно
		// инстанцировать для конкретного T, что по крайней мере сложно (и не нужно, потому что
		// реализации в виде функций-членов в условиях отсутствия операторов приведения типа и
		// наследования хватает с головой).
		IMPLEMENT_MEMBER_COMPARE(Indexed);
	};

	std::set<Indexed> set;
	std::deque<T *> byIndex;
};
