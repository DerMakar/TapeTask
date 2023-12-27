#include "tape.h"
#include "tapesort.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Unvalid arguments ([program name] [input_file] [output_file])";
        return -1;
    }

    yadro::TapeSorter tape_sorter;
    try {
        yadro::DataTape in_tape(argv[1]);
        yadro::DataTape out_tape(argv[2]);
        std::string tmp_tapes_dir("..\\tmp\\");
        std::filesystem::create_directory(tmp_tapes_dir);
        size_t tmp_tape_size = 20;
        tape_sorter.sort(in_tape, out_tape, tmp_tapes_dir, tmp_tape_size);
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return -1;
    }
    return 0;
}