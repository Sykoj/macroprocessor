#include "macrotable.h"

namespace macroprocessor {

	macrotable::macrotable() {
	}

	bool macrotable::contains_macro(const std::string& macro_declaration) {

		return defined_macros_.find(macro_declaration) != defined_macros_.cend() ? true : false;
	}

	macrotable::~macrotable() {
	}

}