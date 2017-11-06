#include "macroprocessor.h"
#include <iostream>

using namespace std;

int main() {

	macroprocessor::macroprocessor macro_processor{ cout };
	char c;

	while (true) {
		
		cin.get(c);
		if (cin.fail()) break;
		macro_processor.process_input(c);
	}

    return 0;
}