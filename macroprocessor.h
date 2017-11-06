#ifndef  MACROPROCESSOR_H_
#define  MACROPROCESSOR_H_

#include <ostream>
#include "macrotable.h"

namespace macroprocessor {

	class macroprocessor {

	public:
		enum class state { text, possible_macro_declaration, macro_declaration, macro_definition, input_format_error };
		/**
		 * Creates new instance of processor.
		 *
		 * @param output Output stream for processor.
		 */
		macroprocessor(std::ostream& output);

		/**
		 * Multiprocessor process single character.
		 * Can write out processed text from buffer into output stream
		 * specified and added in constructor.
		 *
		 * @param c Character that will be processed.
		 */
		void process_input(const char c);

		~macroprocessor();
	private:
		state process_text_input();
		state recognize_macro_declaration();
		state declare_macro();
		state define_macro();

		// Returns macro definition if this word exists as macro declaration or word
		std::string get_representation_of(const std::string& word);

		std::ostream& output_;
		std::string current_macro_declaration_;
		macrotable defined_macros_;						// Table of declared and defined macros
		state state_;
		char previous_char_ = '\r';						// Set to '\r' because of required initialization, has no effect on state of processor or output 
		char current_char_;
		std::string stringbuilder_;
		bool hash_conflict_;							// Flag for detecting if previous '#' was already used in macro end detection
		bool is_start_of_definition_;					// Flag for not saving ' ' as first letter after declaration into definition string
	};

}
#endif