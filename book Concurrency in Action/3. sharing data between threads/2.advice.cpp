#include <list>
#include <mutex>
#include <algorithm>

// Advice:
// Do not pass pointers and references to protected data outside the scope of the lock in any way,
// be it returning from a function, storing it in memory visible from outside,
// or passing it as an argument to a user-defined function.

class some_data {
private:
	int a;
	std::string b;
public:
	void do_something();
};

class data_wrapper {
private: 
	some_data data;
	std::mutex m;
public:
	template<typename Function>
	void process_data(Function func) {
		std::lock_guard<std::mutex> l(m);
		func(data);  // secured data transfered to a custom function
	}
};

some_data* unprotected;

void malicious_function(some_data& protected_data) {
	unprotected = &protected_data;
}

data_wrapper x;

void foo() {
	x.process_data(malicious_function); // malicious functions in class
	unprotected->do_something();		// access protected data
}