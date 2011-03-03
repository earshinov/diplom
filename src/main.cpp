#include "tests.h"

#include <cstdlib>
#include <iostream>
using namespace std;

static void usage(ostream & o) {
	o
		<< "ИСПОЛЬЗОВАНИЕ: diplom КОМАНДА АРГУМЕНТЫ\n" << "\n"
	    << "Доступные команды:\n" << "\n"
	    << "  test - запустить модульные тесты\n"
	    << "  help - показать это сообщение\n";
}

int main(int argc, char ** argv) {
	if (argc != 2) {
		usage(std::cerr);
		return EXIT_FAILURE;
	}

	const char * const cmd = argv[1];
	if (strcmp(cmd, "-h") == 0 || strcmp(cmd, "--help") == 0 ||
		strcmp(cmd, "help") == 0 || strcmp(cmd, "/?") == 0)
	{
		usage(std::cout);
		return EXIT_SUCCESS;
	}
	else if (strcmp(cmd, "test") == 0) {
		return runTests(1 /* argc */, argv);
	}
	else {
		std::cerr << "Неизвестная команда: '" << cmd << "'\n";
		usage(std::cerr);
		return EXIT_FAILURE;
	}
}
