#include "exceptions.h"

namespace yadro {
	OpenFileExcept::OpenFileExcept()
		: message("can't open file") {
	}

	OpenFileExcept::OpenFileExcept(const char* message)
		: message(message) {

	}

	const char* OpenFileExcept::what() const {
		return message;
	}

	EmptyDataExcept::EmptyDataExcept()
		: message("Tape is empty") {
	}

	EmptyDataExcept::EmptyDataExcept(const char* message)
		: message(message) {
	}

	const char* EmptyDataExcept::what() const {
		return message;
	}
}// namespace yadro