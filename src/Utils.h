#pragma once

#define FOREACH_RANGE(vartype, var, count) { \
	for (vartype var = 0; var < count; ++var) {

#define FOREACH_ITER(var, container) { auto cont = container; \
	for (auto it = cont.begin(), end = cont.end(); it != end; ++it) {

#define FOREACH(vardecl, container) { auto cont = container; \
	for (auto it = cont.begin(), end = cont.end(); it != end; ++it) { vardecl = *it;

#define FOREACH_END() } }

#define IMPLEMENT_COMPARE(type) \
bool operator<(const type & first, const type & second) { return first.compareTo(second) < 0; } \
bool operator<=(const type & first, const type & second) { return first.compareTo(second) <= 0; } \
bool operator==(const type & first, const type & second) { return first.compareTo(second) == 0; } \
bool operator!=(const type & first, const type & second) { return first.compareTo(second) != 0; } \
bool operator>=(const type & first, const type & second) { return first.compareTo(second) >= 0; } \
bool operator>(const type & first, const type & second) { return first.compareTo(second) > 0; }
