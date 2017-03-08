/*
Contributors: Jing Chen Ying Guo
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
using namespace std;

int t_cs = 6; //time to take a context switch
int t_slice = 94;

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

string printqueue(vector<string>& queue){
	string str = "Q";
	if (queue.empty())
		str = str + " <empty>";
	for (size_t i = 0; i < queue.size(); i++)
		str = str + ' ' + queue[i];
	return str;
}


void fcfs(vector<vector<string> > data){
	cout << "time 0ms: Simulator started for FCFS [Q <empty>]" << endl;

	vector<vector<int> > vec; //[current time, burst times]
	for (int i = 0; i < data.size(); i++){
		vector<int> tmp;
		tmp.push_back(atoi(data[i][1].c_str()));
		tmp.push_back(atoi(data[i][3].c_str()));
		vec.push_back(tmp);
	}
	
	vector<string> queue;
	vector<string> finish;
	string run = "";

	while (true) {
		//************************************
		//check if everything finishes
		int check = 0;
		for (int i = 0; i < vec.size(); i++){
			if (vec[i][1] != 0)	check = 1;
		}
		if (check == 0 && finish.size() == data.size()) break;
		
		//************************************
		//Find the minimize value
		int min = vec[0][0];
		for (int i = 0; i < vec.size(); i++){
			if (find(finish.begin(), finish.end(), data[i][0]) == finish.end()){
				if (find(finish.begin(), finish.end(), data[0][0]) != finish.end())
					min = vec[i][0];
				if (vec[i][0] <  min)
					min = vec[i][0];
			}
		}
		
		// for (int i = 0; i < vec.size(); i++){
		// 	cout << vec[i][0] << " " << vec[i][1] << endl;
		// }


		//************************************
		//The min could mean:
		//1. It is the new process or start using the CPU
		//2. It complete the task, we add the waiting time
		for (int j = 0; j < vec.size(); j++){
			if (vec[j][0] == min){
				// new process
				if (vec[j][1] == atoi(data[j][3].c_str())){
					if (find(queue.begin(), queue.end(), data[j][0]) == queue.end()){
						queue.push_back(data[j][0]);
						printf("time %dms: Process %s arrived and added to ready queue [%s]\n", 
								vec[j][0], data[j][0].c_str(), printqueue(queue).c_str());
					}
				} else if (data[j][0] == run){
					if (vec[j][1] == 0){
						printf("time %dms: Process %s terminated [%s]\n", 
								vec[j][0], data[j][0].c_str(), printqueue(queue).c_str());
						finish.push_back(data[j][0]);
						vec[j][0] += t_cs/2;
						for (int k = 0; k < queue.size(); k++){
							for (int m = 0; m < data.size(); m++){
								if (queue[k] == data[m][0]){
									vec[m][0] = vec[j][0];
								}
							}
						}

					} else {
						if (vec[j][1] > 1)
							printf("time %dms: Process %s completed a CPU burst; %d bursts to go [%s]\n",
									vec[j][0], data[j][0].c_str(), vec[j][1], printqueue(queue).c_str());
						else 
							printf("time %dms: Process %s completed a CPU burst; %d burst to go [%s]\n",
									vec[j][0], data[j][0].c_str(), vec[j][1], printqueue(queue).c_str());
						printf("time %dms: Process %s switching out of CPU; will block on I/O until time %dms [%s]\n",
								vec[j][0], data[j][0].c_str(), vec[j][0] + atoi(data[j][4].c_str()) + t_cs/2, printqueue(queue).c_str());
						for (int k = 0; k < queue.size(); k++){
							for (int m = 0; m < data.size(); m++){
								if (queue[k] == data[m][0]){
									vec[m][0] = vec[j][0] + t_cs/2;
								}
							}
						}
						vec[j][0] += atoi(data[j][4].c_str()) + t_cs/2;
					}
					run = "";
					//vec[j][0] += t_cs/2;
				} else {
					if (vec[j][1]!=0 && find(queue.begin(), queue.end(), data[j][0]) == queue.end()){
						queue.push_back(data[j][0]);
						printf("time %dms: Process %s completed I/O; added to ready queue [%s]\n", 
								vec[j][0], data[j][0].c_str(), printqueue(queue).c_str());
						vec[j][0] += t_cs/2;
						for (int k = 0; k < queue.size(); k++){
							for (int m = 0; m < data.size(); m++){
								if (queue[k] == data[m][0]){
									vec[m][0] = vec[j][0];
								}
							}
						}
					}

				}
			}
		}

		if (run != "" && queue.size() != 0){
			int time;
			for (int k = 0; k < data.size(); k++){
				if (run == data[k][0]){
					time = vec[k][0];
					break;
				}
			}
			for (int k = 0; k < queue.size(); k++){
				for (int m = 0; m < data.size(); m++){
					if (queue[k] == data[m][0] && vec[m][0] < time){
						vec[m][0] = time;
					}
				}
			}

		}

		// run the process
		if (run == "" && queue.size() != 0){
			run = queue[0];
			
			int pos = 0;
			int time = 0;
			for (int k = 0; k < data.size(); k++){
				if (run == data[k][0]){
					time = vec[k][0] + t_cs/2;
					vec[k][0] = time + atoi(data[k][2].c_str());
					vec[k][1]--;
					pos = k;
					break;
				}
			}

			queue.erase(queue.begin());

			for (int k = 0; k < queue.size(); k++){
				for (int m = 0; m < data.size(); m++){
					if (queue[k] == data[m][0] && vec[m][0] < vec[pos][0]){
						vec[m][0] = vec[pos][0];
						break;
					}
				}
			}

			//cout << vec[pos][0] << endl;
			printf("time %dms: Process %s started using the CPU [%s]\n", 
					time, data[pos][0].c_str(), printqueue(queue).c_str());

			
			
		}
	}

	int max = vec[0][0];
	for (int i = 0; i < vec.size(); i++){
		if (vec[i][0] >  max)
			max = vec[i][0];
	}

	printf("time %dms: Simulator ended for FCFS\n", max);
}

struct process{
	string proc_id;
	int arrival;
	int cpu_burst;
	int burst_num;
	int io_time;

	//run time statement
	int cur_burst;
	int cur_used_cpu_time;
	int last_use_cpu_start_time;
	int cur_burst_remaining;

	int wait_io_start_time;
	
	int enqueue_ready_time;

	process(string proc_id, int arrival,int cpu_burst, int burst_num, int io_time):
	proc_id(proc_id), arrival(arrival),cpu_burst(cpu_burst),burst_num(burst_num),io_time(io_time){
		cur_burst=-1;
		cur_used_cpu_time=0;
		cur_burst_remaining = cpu_burst;
		wait_io_start_time=-1;
		last_use_cpu_start_time = -1;
	}
};

ostream& operator<<(ostream& out, const process& p){
	out<<p.proc_id<<","<<p.arrival<<","<<p.cpu_burst<<","<<p.burst_num<<","<<p.io_time;
	return out;
}

void convert_process(vector<vector<string> >&data, vector<process>& processes){
	for(size_t k=0; k < data.size(); ++k){
		vector<string>& process_data = data[k];
		string proc_id = process_data[0];
		stringstream ss;
		ss<<process_data[1]<<" "<<process_data[2]<<" "<<process_data[3]<<" "<<process_data[4]<<endl;
		int arrival, burst, num, io;
		ss>>arrival>>burst>>num>>io;
		processes.push_back(process(proc_id, arrival, burst, num, io));
	}
}

vector<string>::iterator get_shortest_remaining(vector<string>& queue, map<string,process*>& id2pr){
	vector<string>::iterator min = queue.begin();
	int min_remaining = id2pr[*min]->cur_burst_remaining;
	for(vector<string>::iterator iter = queue.begin();
			iter != queue.end(); ++iter){
		if(id2pr[*iter]->cur_burst_remaining < min_remaining){
			min_remaining = id2pr[*iter]->cur_burst_remaining;
			min = iter;
		}
	}
	return min;
}

void srt_enqueue(int stime, vector<string>& ready_queue, string& proc, map<string, process*> &id2pr){
	id2pr[proc]->enqueue_ready_time = stime;
	ready_queue.push_back(proc);
	for(size_t k=ready_queue.size() - 1; k > 0; --k){
		if(id2pr[ready_queue[k]]->cur_burst_remaining<id2pr[ready_queue[k-1]]->cur_burst_remaining
		||(id2pr[ready_queue[k]]->cur_burst_remaining==id2pr[ready_queue[k-1]]->cur_burst_remaining && ready_queue[k] < ready_queue[k-1])){
			string s = ready_queue[k];
			ready_queue[k] = ready_queue[k-1];
			ready_queue[k-1] = s;
		}else
			break;
	}
}

bool do_process_cpu_burst(int now, process* pr, vector<string>& ready_queue, vector<process*>& blocked_queue, bool* dead){
	bool cpu_burst = false;
	*dead = false;
	if(pr){
		if(--pr->cur_burst_remaining == 0){
			cpu_burst = true;
			string proc_id = pr->proc_id;
			assert(pr->burst_num >= pr->cur_burst);
			size_t left_burst = pr->burst_num  - pr->cur_burst - 1;
			if(left_burst)
				cout<<"time "<<now<<"ms: Process "<<proc_id<<" completed a CPU burst; ";
			else
				cout<<"time "<<now<<"ms: Process "<<proc_id<<" terminated ";
			if(left_burst > 1)
				cout<<left_burst<<" bursts to go ";
			else if(left_burst == 1)
				cout<<left_burst<<" burst to go ";
			cout<<"["<<printqueue(ready_queue)<<"]"<<endl;
			if(left_burst){
				pr->wait_io_start_time = now + t_cs/2;
				pr->cur_burst_remaining = pr->cpu_burst;
				blocked_queue.push_back(pr);
				cout<<"time "<<now<<"ms: Process "<<proc_id<<" switching out of CPU;";
				cout<<" will block on I/O until time "<<(pr->wait_io_start_time + pr->io_time)<<"ms ";
				cout<<"["<<printqueue(ready_queue)<<"]"<<endl;
			}
			*dead = 0 == left_burst;
		}
	}
	return cpu_burst;
}

process* do_io_complete(int stime, process* current, map<string, process*> &id2pr, vector<process*>& blocked_queue, vector<string>& ready_queue){
	vector<process*> io_completed_vec;
	string min_remaining="";
	int remaining=0;
	
	//select all processes completed io
	for(vector<process*>::iterator iter=blocked_queue.begin();iter!=blocked_queue.end();){
		process* pr = *iter;
		assert(pr!=NULL);
		if(stime == pr->wait_io_start_time + pr->io_time){
			//cout<<stime<<"ms: "<<pr->proc_id<<" complete io"<<endl;
			io_completed_vec.push_back(pr);
			iter = blocked_queue.erase(iter);
			if(min_remaining.empty() || remaining > pr->cpu_burst){
				min_remaining = pr->proc_id;
				remaining = pr->cpu_burst;
			}
		}else
			++iter;
	}
	
	process* ready_process = NULL;
	//min remaining process use cpu, others are pushed into ready queue
	for(vector<process*>::iterator iter=io_completed_vec.begin();iter!=io_completed_vec.end();++iter){
		if((*iter)->proc_id != min_remaining || (current ==NULL || current->cur_burst_remaining <= remaining)){
			//ready_queue.push_back((*iter)->proc_id);
			srt_enqueue(stime, ready_queue, (*iter)->proc_id, id2pr);
			cout<<"time "<<stime<<"ms: Process "<<(*iter)->proc_id<<" completed I/O;";
			cout<<" added to ready queue ["<<printqueue(ready_queue)<<"]"<<endl;
		}else
			ready_process = *iter;
	}
	return ready_process;
}

process* do_arrival(int stime, process* current, map<string, process*> &id2pr, vector<process>& processes, vector<string>& ready_queue){
	vector<process*> arrive_vec;
	string min_remaining;
	int remaining=0;
	
	for(size_t k=0; k < processes.size(); ++k){
		if(processes[k].arrival != stime)
			continue;
		process* pr = &processes[k];
		arrive_vec.push_back(pr);
		if(min_remaining.empty() || remaining > pr->cpu_burst){
			min_remaining = pr->proc_id;
			remaining = pr->cpu_burst;
		}
	}
	
	process* ready_process = NULL;
	//min remaining process use cpu, others are pushed into ready queue
	for(vector<process*>::iterator iter=arrive_vec.begin();iter!=arrive_vec.end();++iter){
		if((*iter)->proc_id != min_remaining || (current == NULL || current->cur_burst_remaining <= remaining)){
			//ready_queue.push_back((*iter)->proc_id);
			srt_enqueue(stime, ready_queue, (*iter)->proc_id, id2pr);
			cout<<"time "<<stime<<"ms: Process "<<(*iter)->proc_id<<" arrived and";
			cout<<" added to ready queue ["<<printqueue(ready_queue)<<"]"<<endl;
		}else
			ready_process = *iter;
	}
	return ready_process;
}

void srt(vector<vector<string> >& data, ostream& stats_stream){
	cout << "time 0ms: Simulator started for SRT [Q <empty>]" << endl;
	int stime = 0;
	int burst_time=0,wait_time=0, turnaround_time=0, ctx_switch=0,preemption=0;
	int wait_num = 0;
	
	vector<process> processes;
	vector<string> ready_queue;
	vector<process*> blocked_queue;
	process* current_process=NULL;
	map<string, process*> id2pr;

	size_t n = data.size(), //process num
		   m = 1; //cpu number
	size_t total_burst = 0;

	//
	convert_process(data, processes);
	for(size_t k=0;k < n;++k){
		//cout<<k<<":"<<processes[k]<<endl;
		id2pr[processes[k].proc_id] = &processes[k];
		burst_time += processes[k].cpu_burst * processes[k].burst_num;
		total_burst += processes[k].burst_num;
	}


	size_t alive = processes.size();
	for(;;++stime){
		//cpu burst completion
		bool cpu_burst = false;
		bool dead=false;
		
		//cout<<stime<<"ms: do_process_cpu_burst"<<endl;
		cpu_burst = do_process_cpu_burst(stime, current_process, ready_queue, blocked_queue, &dead);
		if(cpu_burst)
			current_process = NULL;
		if(dead)
			--alive;
		if(!alive)
			break;
		
		//cout<<stime<<"ms: do_io_complete"<<endl;
		//io burst completion
		process* io_pr = do_io_complete(stime, current_process, id2pr, blocked_queue, ready_queue);
		
		//arrival
		//cout<<stime<<"ms: do_arrival"<<endl;
		//cout<<"do_arrival"<<endl;
		process* arrived_pr = do_arrival(stime, current_process, id2pr, processes, ready_queue);

		process* switch_pr = NULL;
		if(io_pr!=NULL && arrived_pr!= NULL){
			if(io_pr->cpu_burst <= arrived_pr->cpu_burst){
				srt_enqueue(stime, ready_queue, arrived_pr->proc_id, id2pr);
				//ready_queue.push_back(arrived_pr->proc_id);
				cout<<"time "<<stime<<"ms: Process "<<arrived_pr->proc_id<<" arrived and added to ready queue ";
				cout<<printqueue(ready_queue)<<endl;
				switch_pr = io_pr;
				
			}else{
				//ready_queue.push_back(io_pr->proc_id);
				srt_enqueue(stime, ready_queue, io_pr->proc_id, id2pr);
				cout<<"time "<<stime<<"ms: Process "<<io_pr->proc_id<<" completed I/O; added to ready queue ";
				cout<<printqueue(ready_queue)<<endl;
				switch_pr = arrived_pr;
			}
		}else if(io_pr)
			switch_pr = io_pr;
		else
			switch_pr = arrived_pr;
		
		//cout<<stime<<"ms: start to switch, current="<<current_process<<endl;
		
		if(current_process){
			if(switch_pr==NULL)
				continue;
			
			//cout<<stime<<"ms: start switch, current!=NULL"<<endl;
			
			if(switch_pr == io_pr){
				cout<<"time "<<stime<<"ms: Process "<<io_pr->proc_id<<" completed I/O and will preempt "<<current_process->proc_id;
				cout<<" ["<<printqueue(ready_queue)<<"]"<<endl;
			}else{
				cout<<"time "<<stime<<"ms: Process "<<switch_pr->proc_id<<" arrived and will preempt "<<current_process->proc_id;
				cout<<" ["<<printqueue(ready_queue)<<"]"<<endl;
			}
			srt_enqueue(stime, ready_queue, current_process->proc_id, id2pr);
			//ready_queue.push_back(current_process->proc_id);
			current_process = switch_pr;
			++current_process->cur_burst;
			cout<<"time "<<(stime + t_cs)<<"ms: Process "<<current_process->proc_id<<" started using the CPU";
			if(current_process->cur_burst_remaining < current_process->cpu_burst)
				cout<<" with "<<current_process->cur_burst_remaining<<"ms remaining";
			cout<<" ["<<printqueue(ready_queue)<<"]"<<endl;
			stime += t_cs;
			
			++ctx_switch;
			++preemption;
			
			//cout<<stime<<"ms: finish switch, current!=NULL"<<endl;

		}else{
			int skip_time = cpu_burst?t_cs:t_cs/2;
			if(switch_pr==NULL){
				
				vector<string>::iterator min = get_shortest_remaining(ready_queue, id2pr);
				if(min != ready_queue.end()){
					current_process = id2pr[*min];
					wait_time += stime - current_process->enqueue_ready_time;
					++wait_num;
					ready_queue.erase(min);
					cout<<"time "<<(stime + skip_time)<<"ms: Process "<<current_process->proc_id<<" started using the CPU";
					if(current_process->cur_burst_remaining == current_process->cpu_burst)
						++current_process->cur_burst;
					else
						cout<<" with "<<current_process->cur_burst_remaining<<"ms remaining";
					cout<<" ["<<printqueue(ready_queue)<<"]"<<endl;
					stime += skip_time;
					++ctx_switch;
				}else{
					continue;
				}
				continue;
			}else if(switch_pr == io_pr){
			}else{
			}
			
			current_process = switch_pr;
			++current_process->cur_burst;
			cout<<"time "<<(stime + skip_time)<<"ms: Process "<<current_process->proc_id<<" started using the CPU";
			cout<<" ["<<printqueue(ready_queue)<<"]"<<endl;
			stime += skip_time;
			++ctx_switch;
			
			//cout<<stime<<"ms: finish switch, current == NULL"<<endl;

		}
		
	}

	stime += t_cs/2;
	cout<<"time "<<stime<<"ms: Simulator ended for SRT"<<endl;


	stats_stream<<"Algorithm SRT"<<endl;

	stats_stream<<"-- average CPU burst time: "<<fixed<<setprecision(2)<<(double)burst_time/total_burst/m<<" ms"<<endl;
	stats_stream<<"-- average wait time: "<<fixed<<setprecision(2)<<(double)wait_time/wait_num<<" ms"<<endl;
	stats_stream<<"-- average turnaround time: "<<turnaround_time<<" ms"<<endl;
	stats_stream<<"-- total number of context switches: "<<ctx_switch<<endl;
	stats_stream<<"-- total number of preemptions: "<<preemption<<endl;
}






int main(int argc, char *argv[]){

	if (argc != 2) {
		cerr << "ERROR: Invalid arguments" << endl;
		cerr << "USAGE: ./a.out <input-file> <stats-output-file>" << endl;
		return -1;
	}

	
	//Read the txt file and store the information in vectors
	vector<vector<string> > data;
	if (!readfile(argv[1], data))
		return -1;

	ofstream stat_stream(argv[2]);

	fcfs(data);

	//srt(data, stat_stream);

	//rr(data, stat_stream);

	stat_stream.close();

	return 0;
}