#include <chrono>
#include <assert.h>
#include <future>

std::chrono::system_clock sc;
std::chrono::steady_clock stc;
std::chrono::high_resolution_clock hrc;

int asserts() {	assert(sc.is_steady() == false);}

int functionality() {auto current_time = sc.now();}

// time intervals:
std::chrono::duration<double, std::ratio<1, 1000>> time_interval;
std::chrono::nanoseconds n;
std::chrono::microseconds m;
std::chrono::milliseconds mm;
std::chrono::seconds sss;
std::chrono::minutes mmm;
std::chrono::hours h;

// typedefs for number degrees:
// for 10^-18
std::atto a;
// for 10^18
std::exa e;

// explicit time casts:
std::chrono::milliseconds ms(54802);
std::chrono::seconds s = 
			std::chrono::duration_cast<std::chrono::seconds>(ms);

// arifmetical operations exists on durations

int some_task() {};

// setting the expectation of the readiness of the future result for a certain interval:
void test1() {
	std::future<int> f = std::async(some_task);
	if (f.wait_for(std::chrono::milliseconds(35)) == std::future_status::ready)
		do_something_with(f.get());
}


// time moments: std::chrono::time_point<>

std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes> d1;

void test2() {
	auto start = std::chrono::high_resolution_clock::now();
	// do_something();
	auto stop = std::chrono::high_resolution_clock::now();

	auto seconds = std::chrono::duration<double, std::chrono::seconds>(stop-start).count();


}