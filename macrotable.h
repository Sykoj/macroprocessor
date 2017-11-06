#ifndef  MACROTABLE_H_
#define  MACROTABLE_H_

#include <string>
#include <unordered_map>

namespace macroprocessor {

	class macrotable {
	public:
		macrotable();
		bool contains_macro(const std::string& macro_declaration);
		std::string& operator[](const std::string& macro_declaration) {

			return defined_macros_[macro_declaration];
		}
		~macrotable();
	private:
		std::unordered_map<std::string, std::string> defined_macros_;
	};
}

#endif