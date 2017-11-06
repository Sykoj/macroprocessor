 #include "macroprocessor.h"

namespace macroprocessor {

	macroprocessor::macroprocessor(std::ostream& output) : output_(output), state_(state::text) {
	}

	void macroprocessor::process_input(const char c) {

		current_char_ = c;

		switch (state_) {

		case state::text: {
			state_ = process_text_input();
			break;
		}
		case state::possible_macro_declaration: {
			state_ = recognize_macro_declaration();
			break;
		}
		case state::macro_declaration: {
			state_ = declare_macro();
			break;
		}
		case state::macro_definition: {
			state_ = define_macro();
			break;
		}
		case state::input_format_error: {

			return;
		}
		}
		previous_char_ = current_char_;
	}


	/**
	 * Process text input, change state:
	 *		If character is '#' -> state::possible_macro_declaration
	 */
	macroprocessor::state macroprocessor::process_text_input() {

		if (current_char_ == '#') {												// If current character '#', is possible macro declaration

			if (previous_char_ == ' ') {										// Previous char was ' ' => buffer is free

				stringbuilder_ += previous_char_;								// Adds " #" into buffer, handled in next state
				stringbuilder_ += current_char_;
			}
			else {

				if (!stringbuilder_.empty()) {									// Write out buffer, add "#" into buffer, handled in next state
					output_ << get_representation_of(stringbuilder_);
					stringbuilder_.clear();
				}	
				stringbuilder_ += current_char_;
			}

			return state::possible_macro_declaration;
		}

		// Delayed print of ' '
		if (previous_char_ == ' ') {
			output_ << previous_char_;
		}

		// Current character is not '#' case
		if (!isgraph(current_char_)) {											// Character is not printable => write out text buffers

			if (!stringbuilder_.empty()) {
				output_ << get_representation_of(stringbuilder_);
				stringbuilder_.clear();
			}
			if (current_char_ != ' ') output_ << current_char_;					// If ' ', delay the print, otherwise write out unprintable character
		}
		else {																	// Character is printable and not '#' => append to buffer
			stringbuilder_ += current_char_;
		}

		return state::text;
	}

	/**
	 * Expects "#" or " #" in buffer
	 *		If start of macro declaration -> empty buffer && state::macro_declaration
	 *		Else -> keeps buffer and state::text for writing out the buffer
	 */
	macroprocessor::state macroprocessor::recognize_macro_declaration() {

		// Not macro declaration:
		if (!isalpha(current_char_)) {								// Macro declaration must start: "#[isalpha]"

			if (current_char_ != ' ') {								// If ' ', delay print, can be part of other macro declaration if next char '#'
				stringbuilder_ += current_char_;
			}
			output_ << stringbuilder_;
			return state::text;

		}
		// Is macro declaration:
		else {
			stringbuilder_.clear();									// Throw away "#" or " #"
			stringbuilder_ += current_char_;						// Add first character of declaration to buffer for macro name 
			return state::macro_declaration;
		}

	}

	macroprocessor::state macroprocessor::declare_macro() {	// expects first character loaded in the buffer

		if (!isalnum(current_char_)) {

			if (current_char_ == ' ') {

				if (!defined_macros_.contains_macro(stringbuilder_)) {

					defined_macros_[stringbuilder_];
					current_macro_declaration_ = stringbuilder_;
					stringbuilder_.clear();
					is_start_of_definition_ = true;
					return state::macro_definition;
				}
				else {
					return state::input_format_error;
				}
			}
			else return state::input_format_error;
		}
		stringbuilder_ += current_char_;
		return state::macro_declaration;
	}

	macroprocessor::state macroprocessor::define_macro() {	// first whitespace already loaded, process rest until ' #'

		if (current_char_ == '#' && previous_char_ == ' ') {

			current_macro_declaration_ = "";
			hash_conflict_ = true;
			is_start_of_definition_ = false;
			return state::text;
		}

		if (isalpha(current_char_) && previous_char_ == '#') {

			return state::input_format_error;
		}

		if (previous_char_ == ' ' && !is_start_of_definition_) {

			defined_macros_[current_macro_declaration_] += previous_char_;
		}

		if (!isgraph(current_char_)) {

			if (stringbuilder_ != current_macro_declaration_) {
				defined_macros_[current_macro_declaration_] += get_representation_of(stringbuilder_);
				stringbuilder_.clear();
			}
			else {
				return state::input_format_error;
			}

			if (current_char_ != ' ') {	// if ' ', hold space because it can be part of macro end definition
				defined_macros_[current_macro_declaration_] += current_char_;
			}
		}
		else {
			stringbuilder_ += current_char_;
		}

		is_start_of_definition_ = false;
		return state::macro_definition;
	}

	
	// Should only support & ref for ostream, no copy
	std::string macroprocessor::get_representation_of(const std::string& word) {

		if (defined_macros_.contains_macro(word)) {
			return defined_macros_[word];
		}
		else return word;
	}

	macroprocessor::~macroprocessor() {
	}

}