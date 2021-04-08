#include "mpi.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cstdarg>
#include "logger/owl.hpp"
#include <chrono>
#include <thread>

#define BUSY_WAIT true

// run: mpiexec -n 4 projectName.exe
void awaitRequest(int rank, MPI_Request* request) {
#if BUSY_WAIT
	int waitCount = 0;
	int flag = 0;
	MPI_Status status;

	do {
		waitCount++;
		MPI_Test(request, &flag, &status);
	} while (!flag);
	owl << rank << ": tested " << waitCount << " times" << std::endl;
#endif
}

void sendToMany(int numProcs) {
	const int MAX_RAND = 20;
	const int sourceRank = 0;
	long randValue = std::rand() % MAX_RAND;
	MPI_Request sendRequest;

	owl << sourceRank << ": random value is " << randValue << std::endl;

	for (int toRank = 1; toRank < numProcs; toRank++) {
		long valueSent = randValue + toRank;

		MPI_Isend(static_cast<void*>(&randValue), 1, MPI_LONG,
			toRank, 1, MPI_COMM_WORLD, &sendRequest);

		awaitRequest(sourceRank, &sendRequest);
		owl << sourceRank << ": send " << valueSent << " to " << toRank << std::endl;
	}
}

void recieveFromOne(int rank) {
	const int sourceRank = 0;
	long recvBuffer = -999;
	MPI_Request recvRequest;

	MPI_Irecv(static_cast<void*>(&recvBuffer), 1, MPI_LONG,
		sourceRank, 1, MPI_COMM_WORLD, &recvRequest);

	awaitRequest(sourceRank, &recvRequest);
	owl << rank << ": got value " << recvBuffer << std::endl;
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

	std::srand(std::time(nullptr) + rank);

	owl << rank << ": hello (p = " << numProcs << ")" << std::endl;
	if (rank == 0) {
		sendToMany(numProcs);
	}
	else {
		recieveFromOne(rank);
	}
	owl << rank << ": goodbye" << std::endl;

	MPI_Finalize();
}