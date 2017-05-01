/*
Contributors: Jing Chen(chenj32),Ying Guo(guoy5)
*/

#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// proc1 p1_mem p1_arr_time_1/p1_run_time_1 ... p1_arr_time_a/p1_run_time_a

bool readfile(char* file_name, vector<vector<string> >& data){
	ifstream input;
	input.open(file_name);
	if (!input) {
		cerr << "ERROR: Invalid input file format" << endl;
		return false;
	}

	string line;
	while (!input.eof()){
		getline(input, line);
		if (line[0] == '#' || line.length() == 0)
			continue;
		else {
		    vector<string> info;
		    stringstream test(line);
			string str;
			while(getline(test, str, ' ')){
			   info.push_back(str);
			}
		    data.push_back(info);
		}
	}
	input.close();
	return true;
}


int main(int argc, char *argv[]){

	if (argc != 3) {
		cerr << "ERROR: Invalid arguments" << endl;
		cerr << "USAGE: ./a.out <input-file> <stats-output-file>" << endl;
		return EXIT_FAILURE;
	}

	
	//Read the txt file and store the information in vectors
	vector<vector<string> > data;
	if (!readfile(argv[1], data)){
		cerr << "ERROR: Invalid input file format" << endl;
		return EXIT_FAILURE;
	}

	//Create a frame
	vector<vector<string> > frame;
	for (unsigned int i = 0; i < 10; i++){
		vector<string> tmp;
		for (unsigned int j = 0; j < 32; j++){
			if (i == 0 || i == 9)
				tmp.push_back("=");
			else
				tmp.push_back(".");
		}
		frame.push_back(tmp);
	}

	return 0;
}