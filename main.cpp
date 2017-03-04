/*
Contributors: Jing Chen Ying Guo
*/

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

void readfile(char* file_name){
	ifstream input;
	input.open(file_name);
	if (!input) {
		cerr << "ERROR: Cannot open txt file" << endl;
	}

	string line;
	vector<vector<string> > data;


	while (!input.eof()){
		getline(input, line);
		if (line[0] == '#' || line.length() == 0)
			continue;
		else {
		    vector<string> info;
		    stringstream test(line);
			string str;
			while(getline(test, str, '|')){
			   info.push_back(str);
			}
		    data.push_back(info);
		}

	}
	input.close();


	for (int i = 0; i < data.size(); i++){
		for (int j = 0; j < data[i].size(); j++)
			cout << data[i][j] << " ";
		cout << endl;
	}

}




int main(int argc, char *argv[]){

	if (argc != 2) {
		cerr << "ERROR: Need two arguments" << endl;
		return -1;
	}

	//Read the txt file and store the information in vectors
	readfile(argv[1]);


	return 0;
}
