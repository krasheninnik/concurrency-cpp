#include <mutex>

/* функция std::lock() и шаблон класса std::lock_guard покрывают большую часть простых
 случаев блокировки, но иногда этого недостаточно. Поэтому с тандартную библиотеку включен
 так-же шаблон std::unique_lock <-- предостпвляет большую гибкость

 std::unique_lock не обязан владеть ассоциированыым с ним мьютексом.
		// Второй аргумент:
			std::adopt_lock - управление захватом мьютекса
			std::defer_lock - мьютекс не долэен захватываться,
	
	захватить можно будет позже, использя lock() объекта std::unique_lock()
	или передав функции std::lock() объект std::unique_lock.
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
		// захват мьютексов:
		std::lock(lock_a, lock_b);
		swap(lhs.some_detail, rhs.some_detail);
	}
};

// функции std::unique_lock вызывают функции мьютекса, а сами поднимаю флаг, 
// соответствующий владению/невладению мьютексом

// передача владения мьютексом между контентами:
void prepare_data();
void do_something();

std::unique_lock<std::mutex> get_lock() {
	extern std::mutex some_mutex;
	std::unique_lock<std::mutex> lk(some_mutex);
	prepare_data();
	return lk;  // возврат мьютекса
}

void process_data() {
	std::unique_lock<std::mutex> lk(get_lock());
	do_something();
}

/* Наличие возможности освобождать блокировку до уничтожения объекта std::unique_lock
означает, что освобождение можно произвести досрочно в какой-то ветка кода, если
ясно, что блокировка больше не понадобится.
*/