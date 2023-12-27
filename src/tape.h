#pragma once

#include "tapeinter.h"
#include "exceptions.h"
#include "fstream"
#include <iostream>
#include <filesystem>

namespace yadro {
	class DataTape : public TapeInter {
	public:
		DataTape(const std::string& file_name);

		DataTape(std::string&& file_name);

		std::vector<int> ReadFile() const override;

		std::vector<int> ReadData(size_t data_length) const override;

		std::vector<int> ReadData(size_t ignore_length, size_t data_length) const override;

		int ReadFirstElement() const override;

		void EraseFirstElement() override;

		size_t MoveTape(size_t start_pos, size_t num_of_ingnores) const override;

		void WriteData(const std::vector<int>& data) override;

	};
} // namespace yadro

