#include "tape.h"

namespace yadro {
	DataTape::DataTape(const std::string& file_name)
		: TapeInter(file_name) {
	}

	DataTape::DataTape(std::string&& file_name)
		: TapeInter(std::move(file_name)) {
	}

	std::vector<int> DataTape::ReadFile() const {
		try {
			return ReadData(INT64_MAX);
		}
		catch (const std::exception& exc) {
			throw;
		}
		
	}

	std::vector<int> DataTape::ReadData(size_t data_length) const {
		try {
			return ReadData(0, data_length);
		}
		catch (const std::exception& exc) {
			throw;
		}
	}

	std::vector<int> DataTape::ReadData(size_t ignore_length, size_t data_length) const {
		std::ifstream input(file_name_);
		if (input.is_open()) {
			std::vector<int> data;
			std::string str_el;
			size_t curr_num_of_elems = 0;
			input.ignore(ignore_length);
			while (curr_num_of_elems < data_length) {
				char c = static_cast<char> (input.get());

				if (input.eof()) break;

				if (c == ' ') {
					curr_num_of_elems++;
					data.push_back(std::stoi(str_el));
					str_el.clear();
					continue;
				}

				str_el += c;
			}

			if (input.eof() && !str_el.empty()) {
				curr_num_of_elems++;
				data.push_back(std::stoi(std::move(str_el)));
			}

			input.close();

			if (data.empty()) throw EmptyDataExcept();

			return data;
		}
		else {
			throw OpenFileExcept();
		}
	}

	int DataTape::ReadFirstElement() const  {
		try {
			return ReadData(1)[0];
		}
		catch (const std::exception& exc) {
			throw;
		}
	}

	void DataTape::EraseFirstElement() {
		try {
			std::vector<int> data = ReadFile();
			data.erase(data.begin());
			WriteData(data);
		}
		catch (const std::exception& exc) {
			throw;
		}
	}

	void DataTape::WriteData(const std::vector<int>& data) {
		std::ofstream output (file_name_);
		if (output.is_open()) {
			if (!data.empty()) {
				for (size_t i = 0; i + 1 < data.size(); ++i) {
					output << data[i] << ' ';
				}
				output << data[data.size() - 1];
			}
			output.close();
		}
		else {
			throw OpenFileExcept();
		}
	}

	size_t DataTape::MoveTape(size_t start_pos, size_t num_of_ingnores) const {
		std::ifstream input(file_name_);
		if (input.is_open()) {
			std::string str_el;
			size_t curr_num_of_elems = 0;
			size_t ignore_length = 0;
			input.ignore(start_pos);
			while (curr_num_of_elems < num_of_ingnores) {
				char c = static_cast<char> (input.get());

				if (input.eof()) break;
				
				ignore_length++;
				
				if (c == ' ') {
					curr_num_of_elems++;
					continue;
				}
			}
			
			input.close();

			if (ignore_length == 0) throw EmptyDataExcept();

			return ignore_length;
		}
		else {
			throw OpenFileExcept();
		}
	}
	

}//namespace yadro