#pragma once

struct AddProhibitionAutomatonStateRet {

	AddProhibitionAutomatonStateRet(bool inserted, int index):
		inserted(inserted), index(index) {}

	const bool inserted;
	const int index;
};
