#pragma once
#include <exception>
#include <string>

namespace yadro {
	class OpenFileExcept : public std::exception {
	public:
		OpenFileExcept();

		OpenFileExcept(const char* message);

		const char* what() const override;
	private:
		const char* message;
	};

	class EmptyDataExcept : public std::exception {
	public:
		EmptyDataExcept();

		EmptyDataExcept(const char* message);

		const char* what() const override;
	private:
		const char* message;
	};
} // namespace yadro
