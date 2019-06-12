#include <condition_variable>
#include <mutex>
#include <chrono>

std::condition_variable cv;
bool done;
std::mutex m;


// рекомендуемые способ ожидания условной переменной с ограничением по времени.
// когда предикат не указывается
bool wait_loop() {
	auto const timeout = std::chrono::steady_clock::now() +
		std::chrono::milliseconds(500);

	std::unique_lock<std::mutex> lk(m);
	while (!done) {
		if (cv.wait_until(lk, timeout) == std::cv_status::timeout) {
			break;
		}
	}
	return done;

}

/* При использование wait_for() в случае ложных пробуждений потока
	таймер сбрасывается (начинает отсчёт с 0 --> может быть неограничен по времени)
*/