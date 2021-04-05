#include <mutex>

/*
	Идея в том, чтобы разбить приложение на отдельые слои и выявить все мьютексы,
	которые могут быть захвачены в каждом слое.

	Программ будет отказано захватить мьютекс, если она уже удерживает
	какой-то мьютекс изнижележайшего слоя.

	Чтобы проверить это во время выполнения, следует приписать каждому мьютексу номер слоя
	и вести учёт мьютексам, захваченным каждым потоком

	hierarchical_mutex не определён в стандартной бибилиотеке, но может быть выведё как:
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

// инициализация максимумом -> возможность захватить любой мьютекс
// для thread_local в каждом потоке - своя копия
thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);


/* для данного типа можно использовать std::lock_guard<>, т.к  в нём присутствуют
	все три функции члена, необходимые для удовлетворения требований концепции мьютекса:
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

// захватывает high_level_mutex 
// а затем вызывает low_level_func(), в которой захватывается low_lewel_mutex
// здесь всё правильно.
void high_level_func() {
	std::lock_guard<hierarchical_mutex> lk(high_level_mutex); 
	high_level_stuff(low_level_func()); 
}

// соблюдает правила и выполняется беспрепятственно
void thread_a() { 
	high_level_func();
}

hierarchical_mutex other_mutex(100);
void do_other_stuff();

void other_stuff() {
	high_level_func();
	do_other_stuff();
}

// нарушает правила, во время выполнения столкнётся с трудностями
// тк захватывет самый нижний в иерархие mutex, а затем пытается захватить высшие mutex
// --> исключение или аварийное заверение
void thread_b() {
	std::lock_guard<hierarchical_mutex> lk(other_mutex);
		other_stuff();
}