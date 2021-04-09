#include <mutex>

/*  the std::lock() function and the std::lock_guard class template cover most of the simple
    blocking cases, but sometimes that's not enough. Therefore the standard library is included
    also the std::unique_lock<> template - provides more flexibility

    std::unique_lock<> is not required to own the associated mutex.
		// Second argument:
		std::adopt_lock - control the acquisition of the mutex
		std::defer_lock - the mutex should not be captured,

	you can lock it later using lock() of the std::unique_lock() object
	or by passing the std::lock() function a std::unique_lock object.
*/

class some_big_object {};
void swap(some_big_object& lhs, some_big_object& rhs);

class X {
private:
	some_big_object some_detail;
	std::mutex m;

public:
	X (some_big_object cosnt & sd) : some_detail(sd) {};

	friend void swap(X& lhs, X& rhs) {
		if (&lhs == &rhs)
			return;
		std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock);
		std::unique_lock<std::mutex> lock_b(rhs.m, std::defer_lock);
		// capture mutexs:
		std::lock(lock_a, lock_b);
		swap(lhs.some_detail, rhs.some_detail);
	}
};

// std::unique_lock functions call the mutex functions, and I raise the flag myself,
// corresponding to ownership / non-ownership of the mutex

// transfer ownership of the mutex between contents:
void prepare_data();
void do_something();

std::unique_lock<std::mutex> get_lock() {
	extern std::mutex some_mutex;
	std::unique_lock<std::mutex> lk(some_mutex);
	prepare_data();
	return lk;  // return mutex
}

void process_data() {
	std::unique_lock<std::mutex> lk(get_lock());
	do_something();
}

/* Being able to release the lock before the std::unique_lock object is destroyed
means that the release can be made ahead of schedule in some branch of the code, if
it is clear that the lock will no longer be needed.
*/