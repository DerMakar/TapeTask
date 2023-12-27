#pragma once

#include "tape.h"
#include "exceptions.h"
#include "tapesort.h"
#include <cassert>
#include <random>

void TestTapeInit() {
	std::string tapename = "refname";
	yadro::DataTape refinit_tape(tapename);
	assert(refinit_tape.file_name_ == tapename);
	yadro::DataTape moveinit_tape("move name");
	assert(moveinit_tape.file_name_ == "move name");
}

void TestReadFile() {
	std::string filename = "read_test_data.txt";
	std::filesystem::create_directory("..\\..\\tests");
	std::ofstream file ("..\\..\\tests\\" + filename);
	if (!file.is_open()){
		std::cout << "file was not open";
		return;
	}
	std::vector<int> data({ 2, 3, 1, 7, 5, 4, 2 });

	for (int el : data) {
		file << el << ' ';
	}

	file.close();
	yadro::DataTape tape("..\\..\\tests\\" + filename);
	assert(tape.ReadFile() == data);
	size_t data_length = 3;
	assert(tape.ReadData(data_length) == std::vector<int> (data.begin(), data.begin() + data_length));
	assert(tape.ReadFirstElement() == 2);
	size_t ignor = tape.MoveTape(0, 3);
	assert(tape.ReadData(ignor, 2) == std::vector<int>({7, 5}));

	try {
		ignor = tape.MoveTape(0, data.size());
		auto v = tape.ReadData(ignor, 2);
	}
	catch (const yadro::EmptyDataExcept& exc) {
	}

	std::filesystem::remove("..\\..\\tests\\" + filename);
}

void TestReadExc() {
	std::string filename = "empty_tape.txt";
	std::filesystem::create_directory("..\\..\\tests");
	std::ofstream file("..\\..\\tests\\" + filename);
	if (!file.is_open()) {
		std::cout << "file was not open";
		return;
	}
	yadro::DataTape tape("..\\..\\tests\\" + filename + "exc_extra");
	try {
		tape.ReadFile();
	}
	catch (const yadro::OpenFileExcept& exc) {

	}
	try {
		tape.ReadData(3);
	}
	catch (const yadro::OpenFileExcept& exc) {
	}
	yadro::DataTape empty_tape("..\\..\\tests\\" + filename);
	try {
		empty_tape.ReadFile();
	}
	catch (const yadro::EmptyDataExcept& exc) {

	}
	try {
		empty_tape.ReadData(1);
	}
	catch (const yadro::EmptyDataExcept& exc) {
	}
	try {
		empty_tape.EraseFirstElement();
	}
	catch (const yadro::EmptyDataExcept& exc) {
	}

	file.close();
	std::filesystem::remove("..\\..\\tests\\" + filename);
}

void TestWriteData() {
	std::string filename = "outfile.txt";
	std::filesystem::create_directory("..\\..\\tests");
	std::ofstream file("..\\..\\tests\\" + filename);
	if (!file.is_open()) {
		std::cout << "file was not open";
		return;
	}
	yadro::DataTape tape("..\\..\\tests\\" + filename);
	std::vector<int> data({ 2, 3, 1, 7, 5, 4, 2 });
	tape.WriteData(data);
	assert(tape.ReadFile() == data);
	tape.EraseFirstElement();
	assert(tape.ReadFile() == std::vector<int> (data.begin() + 1, data.end()));

	file.close();
	std::filesystem::remove("..\\..\\tests\\" + filename);
}

void TestTapeSort() {
	std::string infile = "sortin_test_data.txt";
	
	std::filesystem::create_directory("..\\..\\tests");
	std::ofstream file("..\\..\\tests\\" + infile);
	if (!file.is_open()) {
		std::cout << "file was not open";
		return;
	}
	std::vector<int> data({ 2, 3, 1, 7, 5, 4, 2 });

	for (size_t i = 0; i < data.size() - 1; ++i) {
		file << data[i] << ' ';
	}
	file << data[data.size() - 1];

	file.close();
	yadro::DataTape intape("..\\..\\tests\\" + infile);
	std::string outfile = "sortout_test_data.txt";
	yadro::DataTape outtape("..\\..\\tests\\" + outfile);
	std::string tmp_tapes_dir = "..\\..\\tests\\tmp_tapes\\";
	std::filesystem::create_directory(tmp_tapes_dir);
	size_t tmp_tape_size = 4;
	yadro::TapeSorter tape_sorter;
	tape_sorter.sort(intape, outtape, tmp_tapes_dir, tmp_tape_size);
	std::sort(data.begin(), data.end());
	assert(outtape.ReadFile() == data);
	data = intape.ReadFile();
	
	intape.WriteData(std::vector<int>());
	outtape.WriteData(std::vector<int>());
	try{
		tape_sorter.sort(intape, outtape, tmp_tapes_dir, tmp_tape_size);
	}
	catch (const std::exception& exc) {
	}

	intape.WriteData(data);
	tmp_tape_size = 1;
	tape_sorter.sort(intape, outtape, tmp_tapes_dir, tmp_tape_size);
	std::sort(data.begin(), data.end());
	assert(outtape.ReadFile() == data);
	
	data = intape.ReadFile();
	intape.WriteData(data);
	tmp_tape_size = 1000;
	tape_sorter.sort(intape, outtape, tmp_tapes_dir, tmp_tape_size);
	std::sort(data.begin(), data.end());
	assert(outtape.ReadFile() == data);

	std::filesystem::remove("..\\..\\tests\\" + outfile);
	std::filesystem::remove("..\\..\\tests\\" + infile);

}

void TestRamdomSort(size_t power) {
	
	std::mt19937 gen(std::random_device{}());
	double level = pow(10, power); //pow(10, 9);
	std::uniform_real_distribution<double> elements(0, level);
	std::uniform_real_distribution<double> num(-level, level);

	int first = static_cast<int> (elements(gen));
	int i = 0;
	std::vector<int> container;
	container.reserve(first);
	std::filesystem::create_directory("..\\..\\tests");
	std::string infile = "randomin_test_data.txt";
	std::ofstream file("..\\..\\tests\\" + infile);
	while(i + 1 < first) {
		int el = static_cast<int>(num(gen));
		container.push_back(el);
		file << el << ' ';
		++i;
	}
	int el = static_cast<int>(num(gen));
	container.push_back(el);
	file << el;
	file.close();
	yadro::DataTape intape("..\\..\\tests\\" + infile);
	std::string outfile = "randomout_test_data.txt";
	yadro::DataTape outtape("..\\..\\tests\\" + outfile);
	std::string tmp_tapes_dir = "..\\..\\tests\\tmp_tapes\\";
	std::filesystem::create_directory(tmp_tapes_dir);
	size_t tmp_tape_size = first / 5;
	yadro::TapeSorter tape_sorter;
	tape_sorter.sort(intape, outtape, tmp_tapes_dir, tmp_tape_size);
	std::sort(container.begin(), container.end());
	assert(outtape.ReadFile() == container);
	
	std::filesystem::remove("..\\..\\tests\\" + outfile);
	std::filesystem::remove("..\\..\\tests\\" + infile);

}


void FullTest() {
	TestTapeInit();
	TestReadFile();
	TestReadExc();
	TestWriteData();
	TestTapeSort();
	for (size_t pow = 1; pow < 5; ++pow) {
		TestRamdomSort(pow);
	}	
}