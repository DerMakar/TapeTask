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
		std::ifstream input(file_name_);
		if (input.is_open()) {
			std::vector<int> data;
			std::string str_el;
			size_t curr_num_of_elems = 0;
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

}//namespace yadro