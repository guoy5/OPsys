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
#include <cstdlib>

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


void next_fit(vector<vector<string> > data, vector<vector<string> > frame_, 
			  vector<vector<int> > arrival_time_, vector<vector<int> > run_time_,
			  ostream& stats_stream){

	cout << "time 0ms: Simulator started (Contiguous -- Next-Fit)" << endl;

	vector<vector<string> > frame = frame_;
	vector<vector<int> > arrival_time = arrival_time_;
	vector<vector<int> > exit_time = run_time_;

	for (unsigned int i = 0; i < exit_time.size(); i++){
		for (unsigned int j = 0; j < exit_time[i].size(); j++){
			exit_time[i][j] += arrival_time[i][j];
		}
	}

	int total_process = atoi(data[0][0].c_str());
	vector<string> in_query;


	for (unsigned int a = 0; a < 2; a++){

		int curr_process = 0;
		int min_arrival_time = arrival_time[0][0];

		int curr_exit_process = 0;
		int min_exit_time = exit_time[0][0];

		for (unsigned int i = 0; i < total_process; i++){
			if (arrival_time[i][0] < min_arrival_time){
				min_arrival_time = arrival_time[i][0];
				curr_process = i;
			}

			if (exit_time[i][0] < min_exit_time){
				min_exit_time = arrival_time[i][0];
				curr_exit_process = i;
			}
		}

		if (min_exit_time <= min_arrival_time){
		}



		if (min_arrival_time <= min_exit_time){

			// cout << curr_process << endl;

			string curr_letter = data[curr_process+1][0];
			int fixed_frame = atoi(data[curr_process+1][1].c_str());
			int arrive_time = arrival_time[curr_process][0];
		
			
			printf("time %dms: Process %s arrived (requires %d frames)\n", arrive_time, curr_letter.c_str(), fixed_frame);
			printf("time %dms: Placed process %s:\n", arrive_time, curr_letter.c_str());

			in_query.push_back(curr_letter);

			//remove the front element of arrival_time
			arrival_time[curr_process].erase(arrival_time[curr_process].begin());

			curr_process++;

			int count = 0;
			for (unsigned int i = 1; i < 9; i++){
				for (unsigned int j = 0; j < 32; j++){
					if (count == fixed_frame)
						break;
					else{
						frame[i][j] = data[curr_process][0];
						count++;
					}
				}
				if (count == fixed_frame)
					break;
			}


			for (unsigned int i = 0; i < frame.size(); i++){
				for (unsigned int j = 0; j < frame[i].size(); j++)
					cout << frame[i][j];
				cout << endl;
			}
		}

	}




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

	//Store the arrivial_time and run_time
	vector<vector<int> > arrival_time;
	vector<vector<int> > run_time;
	for (unsigned int i = 1; i < data.size(); i++){
		vector<int> tmp1;
		vector<int> tmp2;
		for (unsigned int j = 0; j < data[i].size(); j++){
			if (j != 0 && j != 1){
				stringstream test(data[i][j]);
				string str;
				getline(test, str, '/');
				tmp1.push_back(atoi(str.c_str()));
				getline(test, str, '/');
				tmp2.push_back(atoi(str.c_str()));
			}
		}
		arrival_time.push_back(tmp1);
		run_time.push_back(tmp2);
	}


	ofstream stat_stream(argv[2]);

	next_fit(data, frame, arrival_time, run_time, stat_stream);

	stat_stream.close();

	return 0;
}