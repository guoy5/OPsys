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
			while(getline(test, str, '|')){
			   info.push_back(str);
			}
		    data.push_back(info);
		}
	}
	input.close();
	return true;
}


void fcfs(const vector<vector<string> >& data){
	cout << "time 0ms: Simulator started for FCFS [Q <empty>]" << endl;
	vector<vector<int> > vec;
	for (int i = 0; i < data.size(); i++){
		vector<int> tmp;
		for (int j = 1; j < data[i].size(); j++)
			tmp.push_back(atoi(data[i][j].c_str()));
		vec.push_back(tmp);
	}
	
	vector<string> queue;

	while (true){
		int check = 0;
		for (int i = 0; i < vec.size(); i++){
			if (vet[i][2] != 0)
				check = 1;
		}
		if (check = 0) break;

		//************************************

		int min = vec[0][0];
		for (int i = 0; i < vec.size(); i++){
			if (vec[i][0] <  min)
		}




	}





	for (int i = 0; i < vec.size(); i++){
		for (int j = 0; j < vec[i].size(); j++)
			cout << vec[i][j] << " ";
		cout << endl;
	}




}




int main(int argc, char *argv[]){

	if (argc != 2) {
		cerr << "ERROR: Invalid arguments" << endl;
		return -1;
	}

	
	//Read the txt file and store the information in vectors
	vector<vector<string> > data;
	if (!readfile(argv[1], data))
		return -1;

	fcfs(data);


	return 0;
}
