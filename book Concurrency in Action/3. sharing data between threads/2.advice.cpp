#include <list>
#include <mutex>
#include <algorithm>

// advice
// Не передавайте указатели и ссылки на защищённые данные за пределы области видимости
// блокировки никаким способом, будь то возврат из функии, сохранение в видиой извне памяти
// или передача в виде аргумента пользовательской функции.

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
		func(data);  // передача защищённый данных пользовательской функции
	}
};

some_data* unprotected;

void malicious_function(some_data& protected_data) {
	unprotected = &protected_data;
}

data_wrapper x;

void foo() {
	x.process_data(malicious_function); // вредоносная функций в класс
	unprotected->do_something();		// доступ к защищённым данным
}