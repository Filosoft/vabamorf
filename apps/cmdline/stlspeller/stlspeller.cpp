#include "speller.h"
#include <iostream>

/* 2023.05.23 Kerge facelift Renee programmile.
*/

#if defined (UNICODE)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	try {
		vabamorf::SPELLER_UTF8 speller("et.dct");

		for (int i = 1; i < argc; i++) {
			std::cout << argv[i] << " -- ";
			if (speller.spell(argv[i])) {
				std::cout << "OK\n";
			} else {
				std::cout << "Vigane, soovitan:";
				std::vector<std::string> suggestions = speller.suggest(argv[i]);
				for (size_t j = 0; j < suggestions.size(); j++) {
					std::cout << " " << suggestions[j];
				}
				std::cout << "\n";
			}
		}

	} catch (const vabamorf::exception &) {
		std::cerr << "Viga!\n";
	}
	return 0;
}
