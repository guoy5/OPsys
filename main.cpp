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

string printqueue(vector<string> queue){
	string str = "Q";
	if (queue.size() == 0)
		str = str + " <empty>";
	for (int i = 0; i < queue.size(); i++)
		str = str + ' ' + queue[i];
	return str;
}

void fcfs(vector<vector<string> > data){
	cout << "time 0ms: Simulator started for FCFS [Q <empty>]" << endl;
	vector<vector<int> > vec;
	for (int i = 0; i < data.size(); i++){
		vector<int> tmp;
		for (int j = 1; j < data[i].size(); j++)
			tmp.push_back(atoi(data[i][j].c_str()));
		vec.push_back(tmp);
	}
	
	vector<string> queue;
	string run = "";

	for (int i = 0; i < 3; i++){
		//************************************
		//check if everything finishes
		int check = 0;
		for (int i = 0; i < vec.size(); i++){
			if (vec[i][2] != 0)	check = 1;
		}
		if (check == 0) break;

		
		//************************************
		//Find the minimize value
		int min = vec[0][0];
		for (int i = 0; i < vec.size(); i++){
			if (vec[i][0] <  min)
				min = vec[i][0];
		}

		//The number could mean:
		//1. It is the new process or start using the CPU
		//2. It complete the task, we add the waiting time
		//************************************

		for (int i = 0; i < vec.size(); i++){
			if (vec[i][0] == min){
				if (data[i][0] == run){
					printf("time %dms: Process %s completed a CPU burst; %d bursts to go [%s]\n",
							vec[i][0], data[i][0].c_str(), vec[i][2], printqueue(queue).c_str());
					//vec[0][0] += vec[0][3];
					printf("time %dms: Process %s switching out of CPU; will block on I/O until time %dms [%s]\n",
							vec[i][0], data[i][0].c_str(), vec[i][0], printqueue(queue).c_str());
					run = "";
				}

				//else {
					if (vec[i][2] == atoi(data[i][3].c_str())){
						if (find(queue.begin(), queue.end(), data[i][0]) == queue.end()){
							queue.push_back(data[i][0]);
							printf("time %dms: Process %s arrived and added to ready queue [%s]\n", 
								vec[i][0], data[i][0].c_str(), printqueue(queue).c_str());
						}
						
					}
				//}

				

			}
		}

		if (run == "" || queue.size() != 0){
			run = queue[0];
			vec[0][2]--;
			//vec[0][0] += vec[0][1];
			queue.erase(queue.begin());
			printf("time %dms: Process %s started using the CPU [%s]\n", vec[i][0], data[i][0].c_str(), printqueue(queue).c_str());
		}
	}





	// for (int i = 0; i < vec.size(); i++){
	// 	for (int j = 0; j < vec[i].size(); j++)
	// 		cout << vec[i][j] << " ";
	// 	cout << endl;
	// }






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
