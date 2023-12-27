#pragma once

#include <string>
#include <vector>

namespace yadro {
	class TapeInter {
	public:
		TapeInter() = default;

		TapeInter(const std::string& file_name)
			: file_name_(file_name) {
		}

		TapeInter(std::string&& file_name)
			: file_name_(std::move(file_name)) {
		}

		virtual std::vector<int> ReadFile() const = 0;

		virtual std::vector<int> ReadData(size_t data_length) const = 0;

		virtual std::vector<int> ReadData(size_t ignore_length, size_t data_length) const = 0;
		
		virtual int ReadFirstElement() const = 0;
		
		virtual void EraseFirstElement() = 0;

		virtual size_t MoveTape(size_t start_pos, size_t num_of_ingnores) const = 0;

		virtual void WriteData(const std::vector<int>& data) = 0;

		virtual ~TapeInter() = default;

		std::string file_name_;
	};
} //namespace yadro