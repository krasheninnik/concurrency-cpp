#include <iostream>
#include <omp.h>

int main()
{
	
#ifdef _OPENMP
	std::cout
		<< "OpenMP Version: "
		<< _OPENMP / 100 << " (" << _OPENMP % 100 << ")" << std::endl;
#else
	std::cout << "Sequential Version" << std::endl;
#endif

	std::cout << "Available threads amount: " << omp_get_max_threads() << std::endl;
	std::cout << "Threads amount in sequential section: " << omp_get_num_threads() << std::endl;

	std::cout << "Start parallel section: " << std::endl;
	#pragma omp parallel
	{
		std::cout << "Threads amount in parallel section: " << omp_get_num_threads() << std::endl;
		int ID = omp_get_thread_num();
		std::cout << " hello (" << ID << ")" << std::endl;
		std::cout << " world (" << ID << ")" << std::endl;

#pragma omp master
		{
			std::cout << "I'm master thread: " << ID << std::endl;
		}

#pragma omp single
		{
			std::cout << "I'm one of the treads: " << ID << std::endl;
		}
	}
}

