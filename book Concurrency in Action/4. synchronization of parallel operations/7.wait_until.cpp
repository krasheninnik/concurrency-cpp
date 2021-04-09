#include <condition_variable>
#include <mutex>
#include <chrono>

std::condition_variable cv;
bool done;
std::mutex m;


// recomended method for waiting CV with time limitation (when no predicate)
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

//  In the case of spurious wake up wait_for() reset the wait timer. 