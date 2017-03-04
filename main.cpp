/*
Contributors: Jing Chen Ying Guo
*/

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>



int main(int argc, char*argv[]){
  if (argc != 2) {
    std::cout << "Arguments No not correct\n";
    return EXIT_FAILURE;
  }

	std::ifstream infile;
	infile.open(argv[1]);
	std::string line;
	if(!infile){
		fprintf(stderr, "Can not open the file\n" );
		return EXIT_FAILURE;
	}

	infile.close();
	
	



}

