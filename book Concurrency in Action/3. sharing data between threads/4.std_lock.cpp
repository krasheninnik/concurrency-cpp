#include <mutex>

// std::lock помогает избежать взаимоблокировки в случаях, когда нужно
// захватить сразу два или более мьютексов, но она не в силах помочь,
// если mutex's захватываются порознь

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

		// примененеие std::lock для захвата двух mutex's
		std::lock(lhs.m, rhs.m);

		// конструируем std::lock_guard для каждого mutexs,
		// std::adopt_lock сообщает, что mutex's уже захвачены
		std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
		std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
		swap(lhs.some_detail rhs.some_detail);
	}
};