#pragma once

#include <string>

std::string resultsHeader();
void generateOneResult(int l, int m, int bits);
void generateResults(int lStart, int lEnd, int mStart, int mEnd, int bits = 0);
