#include <mutex>

/*
	The idea is to split the application into separate layers and reveal all the mutexes, which can be captured in each layer.

	Programs will refuse to acquire a mutex if it is already holding
	some kind of mutex from the underlying layer.

	To check this at runtime, assign a layer number to each mutex.
	and keep track of the mutexes captured by each thread

	hierarchical_mutex is not defined in the standard library, but can be output as:
*/

class hierarchical_mutex {
private:
	std::mutex internal_mutex;
	unsigned long const hierarchy_value;
	unsigned previous_hierarchy_value;
	static thread_local unsigned long this_thread_hierarchy_value;
	
	void check_for_hierarchy_violation() {
		if (this_thread_hierarchy_value <= hierarchy_value) { // 2
			throw std::logic_error("mutex hierarchy violated");
		}
	}

	void update_hierarchy_value() {
		previous_hierarchy_value = this_thread_hierarchy_value; // 3
		this_thread_hierarchy_value = hierarchy_value;
	}

public:
	explicit hierarchical_mutex(unsigned long value) :
		hierarchy_value(value),
		previous_hierarchy_value(0)
	{};

	void lock() {
		check_for_hierarchy_violation();
		internal_mutex.lock(); // 4
		update_hierarchy_value(); // 5
	}

	void unlock() {
		this_thread_hierarchy_value = previous_hierarchy_value; // 6
		internal_mutex.unlock();
	}

	bool try_lock() {
		check_for_hierarchy_violation();
		if (!internal_mutex.try_lock()) // 7
			return false;
		update_hierarchy_value();
		return true;
	}
};

// Initialization with maximum -> the ability to capture any mutex 
// for thread_local in each thread - its own copy
thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);


/*  for this type, you can use std::lock_guard<>, because it contains
	all three member functions needed to satisfy the requirements of the mutex concept:
	lock(), unlock(), try_lock();
*/	

hierarchical_mutex high_level_mutex(10000); 
hierarchical_mutex low_level_mutex(5000); 

int do_low_level_stuff();

int low_level_func() {
	std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
	return do_low_level_stuff();
}

void high_level_stuff(int some_param);

// capture high_level_mutex 
// then call low_level_func(), in which captured low_lewel_mutex
// there everything is right.
void high_level_func() {
	std::lock_guard<hierarchical_mutex> lk(high_level_mutex); 
	high_level_stuff(low_level_func()); 
}

// follows the rules and is executed without hindrance
void thread_a() { 
	high_level_func();
}

hierarchical_mutex other_mutex(100);
void do_other_stuff();

void other_stuff() {
	high_level_func();
	do_other_stuff();
}

// breaks the rules, will run into difficulties during execution:
// captures the lowest mutex in the hierarchy and then tries to capture the highest mutex
// -> exception or emergency confirmation
void thread_b() {
	std::lock_guard<hierarchical_mutex> lk(other_mutex);
		other_stuff();
}