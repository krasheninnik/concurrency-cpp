#include <mutex>

// std::lock helps to avoid deadlocks in cases where two or more mutexe's
// need to be captured at once, but it can not help if mutex's are captured separately

class some_big_object {};

void swap(some_big_object& lhs, some_big_object& rhs);

class X {
private: 
	some_big_object some_detail;
	std::mutex m;
public:
	X(some_big_object const& sd) :some_detail(sd) {};
	friend void swap(X& lhs, X& rhs) {
		if (&lhs == &rhs)
			return;

		// Using std::lock to capture mutex's
		std::lock(lhs.m, rhs.m);

		// Construct std::lock_guard for each mutexs,
		// std::adopt_lock report, that mutex's are already captured
		std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
		std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
		swap(lhs.some_detail rhs.some_detail);
	}
};