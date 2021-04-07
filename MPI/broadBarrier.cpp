#include "mpi.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cstdarg>
#include "logger/owl.hpp"

// run: mpiexec -n 4 projectName.exe
void roundRobin(int rank, int numProcs) {

}


int main(int argc, char** argv) {

	// set logger:
	OwlSettings settings;
	settings.b_file = false;
	settings.b_stdout = true;
	settings.b_fileline = false;
	OwlInit(settings);
	owl << "check this line" << std::endl;

	int numProcs;
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//assert(numProcs % 2 == 0 && "There cant be odd amount of processes");

	std::cout << rank << ": hello (p = " << numProcs << ")" << std::endl;
	//roundRobin(rank, numProcs);
	//roundRobin(rank, numProcs);
	std::cout << rank << ": goodbye" << std::endl;

	MPI_Finalize();
}