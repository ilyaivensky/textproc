/*
 * main.cc
 *
 * Diver for Decomposer
 *
 *  Created on: Apr 26, 2014
 *      Author: ilyaivensky
 */

#include <fstream>
#include "CLParser.h"
#include "Decomposer.h"

using namespace std;

int main(int argc, char *argv[])
{

	/*****************************
	 *  Get command line arguments
	 *****************************/
	CLParser cmd_line(argc, argv, true);

	string unigram_file_path = cmd_line.get_arg("-u");
	string in_file_path = cmd_line.get_arg("-i");
	string out_file_path = cmd_line.get_arg("-o");

	/*******************************
	 * Process test file
	 *******************************/
	try
	{
		Decomposer decomposer(unigram_file_path);
#if 0
		std::ifstream ifs;

		// enable exception if failed opening file
		ifs.exceptions(ifstream::failbit);
		ifs.open(in_file_path, std::ios::in);

		std::ofstream ofs;

		// enable exception if failed opening file
		ofs.exceptions(ifstream::failbit);
		ofs.open(out_file_path, std::ios::out);

		while (!ifs.eof())
		{
			std::string line;
			std::getline(ifs, line);
			ofs << decomposer.decompose(line) << endl;
		}

		ofs.close();
		ifs.close();
#else
		while (true)
		{
			string token;
			cin >> token;
			cout << decomposer.decompose(token) << endl;;
		}
#endif
	}
	catch (ifstream::failure e)
	{
		cerr << "Failure opening file";
	}

	return 0;
}





