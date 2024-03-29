#include "getopt_long.h"
#include "results.h"
#include "tests/tests.h"
#include "unicode.h"
#include "Utils.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>


static void usage(std::ostream & o) {
	std::stringstream ss;
	ss
		<< "ИСПОЛЬЗОВАНИЕ: diplom КОМАНДА АРГУМЕНТЫ\n" << "\n"
	    << "Доступные команды:\n"
	    << "\n"
	    << "  run  - запустить перебор АВВП\n"
	    << "  test - запустить модульные тесты\n"
	    << "  help - показать это сообщение\n"
	    << "\n"
	    << "Команда run принимает аргументы\n"
	    << "\n"
	    << "  --L-start=ЧИСЛО - Начальное значение размера входной памяти\n"
	    << "                    По умолчанию 0\n"
	    << "  --L-end=ЧИСЛО   - Конечное значение размера входной памяти\n"
	    << "  -L ЧИСЛО        - размер входной памяти\n"
	    << "                    Эквивалентно --L-start=ЧИСЛО и --L-end=ЧИСЛО\n"
	    << "\n"
	    << "  --M-start=ЧИСЛО - Начальное значение размера выходной памяти\n"
	    << "                    По умолчанию 0\n"
	    << "  --M-end=ЧИСЛО   - Конечное значение размера выходной памяти\n"
	    << "  -M ЧИСЛО        - Размер входной памяти\n"
	    << "                    Эквивалентно --M-start=ЧИСЛО и --M-end=ЧИСЛО\n"
	    << "\n"
	    << "  --func=ФУНКЦИЯ  - Функция, с которой начинать перебор, в виде столбца значений\n"
	    << "          Ради читаемости числа в произвольных местах можно разделять точками\n"
	    << "  --one - Выдать информацию только по функции, заданной через аргумент --func\n"
	    << "          Полный перебор в этом случае не запускается\n"
	    << "\n"
	    << "  Пример использования: diplom run -M 1 -L 2 --func=0100.0010.0001.1000\n"
	    << "  В том или ином виде всегда должны указываться\n"
	    << "  размеры входной и выходной памяти\n";
	o << U(ss.str());
}


class BadCommandLineException : public std::exception {
public:

	BadCommandLineException() { }
	explicit BadCommandLineException(const std::string & message) : message(message) { }
	explicit BadCommandLineException(std::string && message) : message(message) { }
	virtual ~BadCommandLineException() throw() { }

	/* override */ const char * what() const throw() { return message.empty() ? 0 : message.c_str(); }

private:

	std::string message;
};


// намеренно копируем std::string function, чтобы использовать метод erase()
static int getFunctionBits(size_t bitCount, std::string function) {
	// удаляем точки
	function.erase(std::remove(function.begin(), function.end(), '.'), function.end());

	// проверяем длину столбца функции
	if (function.size() != bitCount)
		throw BadCommandLineException(
			"Количество аргументов функции, заданной через аргумент --func, "
			"должно соответствовать начальным размерам входной и выходной памяти "
			"(задаются с помощью аргументов --l-start и --m-start).  "
			"Длина столбца функции должна составлять 2^{L+M+1}" );

	// первый символ становится младшим битом
	int bits = 0;
	for(auto it = function.rbegin(); it != function.rend(); ++it) {
		bits <<= 1;
		char c = *it;
		if (c == '1')
			bits++;
		else if (c != '0')
			throw BadCommandLineException(
				"Столбец функции должен состоять только из нулей и единиц, "
				"возможно разделённых точками" );
	}
	return bits;
}


static int handleRunCmd(int argc, char ** argv) {

	option longopts[] = {
		{ "L-start", required_argument, 0, 0 },
		{ "L-end",   required_argument, 0, 0 },
		{ "M-start", required_argument, 0, 0 },
		{ "M-end",   required_argument, 0, 0 },
		{ "func",   required_argument, 0, 0 },
		{ "one",     no_argument, 0, 0 },
		{0}
	};

	int lStart = 0;
	int lEnd = -1;
	int mStart = 0;
	int mEnd = -1;
	int val;
	std::string startFunction;
	bool one = false;

	while (true) {
		int longOptionIndex;
		int flag = getopt_long(argc, argv, "L:M:", longopts, &longOptionIndex);
		if (flag == -1)
			break;
		switch (flag) {
		case 'L':
			val = atoi(optarg);
			if (val < 0) throw BadCommandLineException("Размер входной памяти должен быть неотрицательным");
			lStart = lEnd = val;
			break;
		case 'M':
			val = atoi(optarg);
			if (val < 0) throw BadCommandLineException("Размер выходной памяти должен быть неотрицательным");
			mStart = mEnd = val;
			break;
		case 0:
			switch(longOptionIndex) {
			case 0: // --L-start
				val = atoi(optarg);
				if (val < 0) throw BadCommandLineException("Размер входной памяти должен быть неотрицательным");
				lStart = val;
				break;
			case 1: // --L-end
				val = atoi(optarg);
				if (val < 0) throw BadCommandLineException("Размер входной памяти должен быть неотрицательным");
				lEnd = val;
				break;
			case 2: // --M-start
				val = atoi(optarg);
				if (val < 0) throw BadCommandLineException("Размер выходной памяти должен быть неотрицательным");
				mStart = val;
				break;
			case 3: // --M-end
				val = atoi(optarg);
				if (val < 0) throw BadCommandLineException("Размер выходной памяти должен быть неотрицательным");
				mEnd = val;
				break;
			case 4: // --func
				startFunction = optarg;
				break;
			case 5: // --one
				one = true;
				break;
			default:
				assert(false);
				throw BadCommandLineException();
			}
			break;
		default:
			throw BadCommandLineException();
		}
	}

	if (lEnd == -1) throw BadCommandLineException("Не задан размер входной памяти");
	if (mEnd == -1) throw BadCommandLineException("Не задан размер выходной памяти");
	if (one && startFunction.empty()) throw BadCommandLineException("При использовании --one укажите функцию АВВП с помощью аргумента --func");
	if (!startFunction.empty()) {
		int bits = getFunctionBits(1 << (lStart + mStart + 1), startFunction);
		if (one) {
			if (lStart <= lEnd && mStart <= mEnd) {
				std::cout << resultsHeader() << "\n";
				generateOneResult(lStart, mStart, bits);
			}
		}
		else {
			std::cout << resultsHeader() << "\n";
			generateResults(lStart, lEnd, mStart, mEnd, bits);
		}
	}
	else {
		std::cout << resultsHeader() << "\n";
		generateResults(lStart, lEnd, mStart, mEnd);
	}

	return EXIT_SUCCESS;
}


static int handleCmd(int argc, char ** argv) {
	if (argc < 2) throw BadCommandLineException();

	const char * const cmd = argv[1];
	if (strcmp(cmd, "-h") == 0 || strcmp(cmd, "--help") == 0 ||
		strcmp(cmd, "help") == 0 || strcmp(cmd, "/?") == 0)
	{
		if (argc > 2) throw BadCommandLineException();

		usage(std::cout);
		return EXIT_SUCCESS;
	}
	else if (strcmp(cmd, "test") == 0) {
		return runTests(1 /* argc */, argv);
	}
	else if (strcmp(cmd, "run") == 0) {
		argv[1] = argv[0];
		return handleRunCmd(argc-1, argv+1);
	}
	else throw BadCommandLineException(std::string("Неизвестная команда: '") + cmd + "'");
}


int main(int argc, char ** argv) {
	try {
		return handleCmd(argc, argv);
	}
	catch(const BadCommandLineException & e) {
		if (e.what()) std::cerr << U(e.what()) << "\n";
		usage(std::cerr);
		return EXIT_FAILURE;
	}
}
