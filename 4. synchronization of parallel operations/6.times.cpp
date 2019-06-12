#include <chrono>
#include <assert.h>
#include <future>
// стабильные часы:: постоянная частота & не допусают подведения.

std::chrono::system_clock sc;
std::chrono::steady_clock stc;
std::chrono::high_resolution_clock hrc;

int asserts() {
	assert(sc.is_steady() == false);

}

int functionality() {

	auto current_time = sc.now();


}

// временные интервалы
//  тип представления; дробь, показывающая сколько секунд содержит один интервал
std::chrono::duration<double, std::ratio<1, 1000>> time_interval;
std::chrono::nanoseconds n;
std::chrono::microseconds m;
std::chrono::milliseconds mm;
std::chrono::seconds sss;
std::chrono::minutes mmm;
std::chrono::hours h;

// typedefs for всех определённых в системе СИ степеней 10:
// от 10^-18
std::atto a;
// до 10^18
std::exa e;

// явное преобразование интервалов:
std::chrono::milliseconds ms(54802);
std::chrono::seconds s = 
			std::chrono::duration_cast<std::chrono::seconds>(ms);

// на интервалах пределены арифметичесие операции


int some_task() {};
// задание ожидания готовности будующего результата в течение некоторого интервала:
void test1() {
	std::future<int> f = std::async(some_task);
	if (f.wait_for(std::chrono::milliseconds(35)) == std::future_status::ready)
		do_something_with(f.get());
}


// Моменты времени
// std::chrono::time_point<>

std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes> d1;
// к моменту времени можно прибавить/вычесть интервал
// Можно вычетать моменты времени из друг друга, если он замерены с одинми часами:

void test2() {
	auto start = std::chrono::high_resolution_clock::now();
	// do_something();
	auto stop = std::chrono::high_resolution_clock::now();

	auto seconds = std::chrono::duration<double, std::chrono::seconds>(stop-start).count();


}