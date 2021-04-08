#include "mpi.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cstdarg>
#include "logger/owl.hpp"
#include <chrono>
#include <thread>

// run: mpiexec -n 4 projectName.exe
int broadcast(int rank, int numProcs) {
	const int MAX_RAND = 20;
	long randValue = 0;
	int bradcasterRank = numProcs - 1;

	if (rank == bradcasterRank) {
		std::srand(std::time(nullptr) + rank);
		randValue = std::rand() % MAX_RAND;
		owl << rank << ": broadcasting " << randValue << std::endl;
	}

	MPI_Bcast(static_cast<void*>(&randValue),
		1, MPI_LONG,
		bradcasterRank,
		MPI_COMM_WORLD);

	if (rank != bradcasterRank) {
		owl << rank << ": received " << randValue << std::endl;
	}

	return randValue;
}

void barrier(int rank, int numProcs) {
	std::srand(std::time(nullptr) + rank);
	int napTime = std::rand() + rank*1000;
	owl << rank << ": sleeping " << napTime << std::endl;
	
	std::this_thread::sleep_for(std::chrono::milliseconds(napTime));

	owl << rank << ": enter barrier" << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);
	owl << rank << ": leave barrier" << std::endl;
}

int main(int argc, char** argv) {

	// set logger:
	OwlSettings settings;
	settings.b_file = false;
	settings.b_stdout = true;
	settings.b_fileline = false;
	OwlInit(settings);

	int numProcs;
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	owl << rank << " hello (p = " << numProcs << ")" << std::endl;
	auto randomValue = broadcast(rank, numProcs);
	barrier(rank, randomValue);
	owl << rank << " goodbye" << std::endl;

	MPI_Finalize();
}