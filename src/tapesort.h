#pragma once

#include "tape.h"
#include <algorithm>
#include <unordered_map>

namespace yadro {
	class TapeSorter {
	public:
		void sort(const TapeInter& input_tape
			, const TapeInter& output_tape_
			, const std::string& tmp_tapes_dir
			, const size_t tmp_tapes_size);
	private:
		size_t ParseByTmpTapes(const TapeInter& input_tape
			, const std::string& tmp_tapes_dir
			, const size_t tmp_tapes_size);

		void SortTmpTapes(const std::string& tmp_tapes_dir, size_t num_of_tmp_tapes);

		void WriteSortedTape(const TapeInter& output_tape_
			, const std::string& tmp_tapes_dir
			, size_t num_of_tmp_tapes);
		
	};
}// namespace yadro


