#include "mpi.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

// run: mpiexec -n 4 projectName.exe
void roundRobin(int rank, int numProcs) {
	const int MAX_RAND = 20;
	long int mineRand = 0, prevRand = 0;
	int nextRank = (rank + 1) % numProcs;
	int prevRank = rank == 0 ? numProcs - 1 : rank - 1;
	MPI_Status status;

	std::srand(std::time(nullptr) + rank);
	mineRand = std::rand() % MAX_RAND;
	
	std::cout << rank << ": random is " << mineRand << std::endl;
	
	if (rank % 2 == 0) {
		std::cout << rank << ": sending " << mineRand << " to " << nextRank << std::endl;
		MPI_Send(static_cast<void*>(&mineRand), 1, MPI_LONG, nextRank, 1, MPI_COMM_WORLD);

		std::cout << rank << ": receiveing from " << prevRank << std::endl;
		MPI_Recv(static_cast<void*>(&prevRand), 1, MPI_LONG, prevRank, 1, MPI_COMM_WORLD, &status);
	}
	else {
		std::cout << rank << ": receiveing from " << prevRank << std::endl;
		MPI_Recv(static_cast<void*>(&prevRand), 1, MPI_LONG, prevRank, 1, MPI_COMM_WORLD, &status);

		std::cout << rank << ": sending " << mineRand << " to " << nextRank << std::endl;
		MPI_Send(static_cast<void*>(&mineRand), 1, MPI_LONG, nextRank, 1, MPI_COMM_WORLD);
	}

	std::cout << rank << ": >> I had " << mineRand << ", " << prevRank << " had " << prevRand << std::endl;

}

int main(int argc, char** argv) {
	int numProcs;
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	assert(numProcs % 2 == 0 && "There cant be odd amount of processes");

	std::cout << rank << ": hello (p = " << numProcs << ")" << std::endl;
	roundRobin(rank, numProcs);
	std::cout << rank << ": goodbye" << std::endl;

	MPI_Finalize();
}