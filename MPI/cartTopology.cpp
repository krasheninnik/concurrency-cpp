#include "mpi.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cstdarg>
#include "logger/owl.hpp"
#include <chrono>
#include <thread>

// run: mpiexec -n 8 projectName.exe
// amount processors (-n X) mush be equal to DIMS_0 * DIMS_1

// print scheme of processes mapping on the cartesian grid
void printProcessorsMapping(int coords[], const int dims[], MPI_Comm commCart) {
	for (coords[0] = 0; coords[0] < dims[0]; coords[0]++) {
		std::cout << coords[0] << "|";
		for (coords[1] = 0; coords[1] < dims[1]; coords[1]++) {
			int rank;
			MPI_Cart_rank(commCart, coords, &rank);
			std::cout << " " << rank;
		};
		std::cout << std::endl;
	}
}

// print scheme of processes neighbors
void printProcessorsNeighbors(int rank, MPI_Comm commCart) {
	int upRank, downRank, leftRank, rightRank;
	MPI_Cart_shift(commCart, 0, 1, &upRank, &downRank);
	MPI_Cart_shift(commCart, 1, 1, &leftRank, &rightRank);
	owl << rank << ":\t   " << upRank << std::endl;
	owl << rank << ":\t" << leftRank << " " << rank << " " << rightRank << std::endl;
	owl << rank << ":\t   " << downRank << std::endl;

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

	MPI_Comm commCart;
	const int DIMS_0 = 4;
	const int DIMS_1 = 2;
	const int ndims = 2;
	const int dims[] = { DIMS_0, DIMS_1 };
	const int periodic[] = { 1, 0 };

	MPI_Cart_create(MPI_COMM_WORLD,
		2,
		dims,
		periodic,
		0,
		&commCart);

	// Get Rank from new communicator
	MPI_Comm_rank(commCart, &rank);
	// Get coordinates by new Rank
	int coords[2];
	MPI_Cart_coords(commCart, rank, 2, coords);
	owl << rank << ": coords " << coords[0] << coords[1] << std::endl;
	MPI_Barrier(commCart);

	if (rank == 0) printProcessorsMapping(coords, dims, commCart);
	MPI_Barrier(commCart);

	printProcessorsNeighbors(rank, commCart);

	MPI_Finalize();
}