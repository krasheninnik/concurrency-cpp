#include <iostream>
#include <omp.h>
#include <functional>
#include <vector>
#include <cassert>

constexpr long NUM_STEPS = 100000;

// Rectangle integration
double rectangleIntegration(double a, double b, std::function<double(double)> f, bool parallel = false) {
	double sum = 0;
	double dx = (b - a) / NUM_STEPS;

	if (parallel) {
		for (int i = 0; i < NUM_STEPS; i++) {
			double x = a + ((i + 0.5) * dx);
			sum += f(x);
		}
	}
	else {
#pragma omp parallel for reduction(+:sum)
		for (int i = 0; i < NUM_STEPS; i++) {
			double x = a + ((i + 0.5) * dx);
			sum += f(x);
		}
	}

	sum *= dx;
	return sum;
}

void main()
{
	std::vector<std::function<double(double)>> cases{
		[](double x) {return 4.0 / (1.0 + x * x); },
		[](double x) {return 5; },
		[](double x) {return x; },
		[](double x) {return log2(x); },
	};

	constexpr int a = 0;
	constexpr int b = 1;

	std::cout << "The discrepancy is due to the summation of floating point numbers with different ordering, not a race conditoin:" << std::endl;
	std::cout << "There second case: constant function to prove it or you can play with NUM_STEPS parameter to check this." << std::endl;

	for (auto thecase : cases) {
		double seqResult = rectangleIntegration(a, b, thecase, false);
		double parallelResult = rectangleIntegration(a, b, thecase, true);
		std::cout << "Discrepancy between parallel and sequential calculations: " << (parallelResult - seqResult) << std::endl;
	}
}
