#pragma once

#define FOREACH_RANGE(vartype, var, count) { \
	for (vartype var = 0; var < count; ++var) {

#define FOREACH_ITER(var, container) { auto cont = container; \
	for (auto it = cont.begin(), end = cont.end(); it != end; ++it) {

#define FOREACH(vardecl, container) { auto cont = container; \
	for (auto it = cont.begin(), end = cont.end(); it != end; ++it) { vardecl = *it;

#define FOREACH_END() } }

#define IMPLEMENT_COMPARE(type) \
inline bool operator<(const type & first, const type & second) { return first.compareTo(second) < 0; } \
inline bool operator<=(const type & first, const type & second) { return first.compareTo(second) <= 0; } \
inline bool operator==(const type & first, const type & second) { return first.compareTo(second) == 0; } \
inline bool operator!=(const type & first, const type & second) { return first.compareTo(second) != 0; } \
inline bool operator>=(const type & first, const type & second) { return first.compareTo(second) >= 0; } \
inline bool operator>(const type & first, const type & second) { return first.compareTo(second) > 0; }

#define IMPLEMENT_MEMBER_COMPARE(type) \
bool operator<(const type & other) const { return compareTo(other) < 0; } \
bool operator<=(const type & other) const { return compareTo(other) <= 0; } \
bool operator==(const type & other) const { return compareTo(other) == 0; } \
bool operator!=(const type & other) const { return compareTo(other) != 0; } \
bool operator>=(const type & other) const { return compareTo(other) >= 0; } \
bool operator>(const type & other) const { return compareTo(other) > 0; }

template <typename Iterator, typename F>
void foreach_2_tuples(Iterator begin, Iterator end, F func) {
	for (Iterator first = begin; first != end; ++first)
		for (Iterator second = begin; second != end; ++second)
			if (first != second)
				func(*first, *second);
}
