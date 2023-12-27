#include "tapesort.h"

namespace yadro {
	void TapeSorter::sort(const TapeInter& input_tape
		, const TapeInter& output_tape_
		, const std::string& tmp_tapes_dir
		, const size_t tmp_tapes_size) {
		try {
			size_t tmp_tapes_count = ParseByTmpTapes(input_tape, tmp_tapes_dir, tmp_tapes_size);
			SortTmpTapes(tmp_tapes_dir, tmp_tapes_count);
			WriteSortedTape(output_tape_, tmp_tapes_dir, tmp_tapes_count);
		}
		catch (const std::exception& exc) {
			throw;
		}
	}

	size_t TapeSorter::ParseByTmpTapes(const TapeInter& input_tape
		, const std::string& tmp_tapes_dir
		, const size_t tmp_tapes_size) {
		std::ifstream input(input_tape.file_name_);
		if (input.is_open()) {
			size_t tmp_tape_count = 0;
			std::string str_el;
			while (!input.eof()) {
				std::ofstream tmp_tape(tmp_tapes_dir + "tmp_" + std::to_string(++tmp_tape_count) + ".txt");
				size_t elems_in_tmp = 0;
				std::vector<int> tmp_data;
				while (elems_in_tmp < tmp_tapes_size) {
					char c = static_cast<char> (input.get());

					if (input.eof()) {
						elems_in_tmp++;
						tmp_tape << str_el;
						str_el.clear();
						break;
					}
						
					if (c == ' ') {
						elems_in_tmp++;
						tmp_tape << str_el << ' ';
						str_el.clear();
						continue;
					}

					str_el += c;
				}
				tmp_tape.close();				
			}
			input.close();
			return tmp_tape_count;
		}
		else {
			throw OpenFileExcept();
		}
	}

	void TapeSorter::SortTmpTapes(const std::string& tmp_tapes_dir, size_t num_of_tmp_tapes) {
		size_t tmp_index = 1;
		while (tmp_index <= num_of_tmp_tapes) {
			DataTape tmp_tape(tmp_tapes_dir + "tmp_" + std::to_string(tmp_index++) + ".txt");
			try {
				std::vector<int> tmp_data = tmp_tape.ReadFile();
				std::sort(tmp_data.begin(), tmp_data.end());
				tmp_tape.WriteData(tmp_data);
			}
			catch (const std::exception& exc) {
				throw;
			}
		}
	}

	void TapeSorter::WriteSortedTape(const TapeInter& output_tape_
		, const std::string& tmp_tapes_dir
		, size_t num_of_tmp_tapes) {
		std::ofstream output(output_tape_.file_name_);
		if (output.is_open()) {
			std::unordered_map<size_t, int> tmp_tapes_firsts;
			std::unordered_map<size_t, size_t> tmp_tapes_ignores;
			size_t tmp_index = 1;
			while (tmp_index <= num_of_tmp_tapes) {
				DataTape tmp_tape(tmp_tapes_dir + "tmp_" + std::to_string(tmp_index) + ".txt");
				try {
					tmp_tapes_firsts[tmp_index] = tmp_tape.ReadFirstElement();
					tmp_tapes_ignores[tmp_index] = 0;
				}
				catch (const std::exception& exc) {
					throw;
				}
				++tmp_index;
			}

			while (true) {
				std::pair<int, int> min_first = std::make_pair(-1, INT32_MAX);
				for (const auto& [index, elem] : tmp_tapes_firsts) {
					if (min_first.first == -1 || elem < min_first.second) min_first = { index, elem };
				}

				if (min_first.first == -1) {
					output.close();
					break;
				}

				output << min_first.second << ' ';

				DataTape tmp_tape(tmp_tapes_dir + "tmp_" + std::to_string(min_first.first) + ".txt");

				try {
					tmp_tapes_ignores[min_first.first] += tmp_tape.MoveTape(tmp_tapes_ignores[min_first.first], 1);
					tmp_tapes_firsts[min_first.first] = tmp_tape.ReadData(tmp_tapes_ignores[min_first.first], 1)[0];					
				}
				catch (const EmptyDataExcept& exc) {
					tmp_tapes_firsts.erase(min_first.first);
					std::filesystem::remove(tmp_tapes_dir + "tmp_" + std::to_string(min_first.first) + ".txt");
				}
				catch (const std::exception& exc) {
					output.close();
					throw;
				}
			}
		}
		else {
			throw OpenFileExcept();
		}
	}
}//namespace yadro