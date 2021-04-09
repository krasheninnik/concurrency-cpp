#include <mutex>
#include <condition_variable>
#include <queue>

struct data_chunk {};

std::mutex mut;
std::queue<data_chunk> data_queue; // 1
std::condition_variable data_cond;

void data_preparation_thread() {
	while (more_data_to_prepare()) {
		data_chunk const data = prepare_data();
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(data); // 2

		// notify waiting thread (if someone wait)
		data_cond.notify_one(); // 3
	}
}

// consider spurious wake: process can occasionally awake without notification
void data_processing_thread() {
	while (true) {
		// neded unique_lock (posibility of capture and release mutex)
		std::unique_lock<std::mutex> lk(mut); // 4

		// A lock object and predicate are passed in wait
		data_cond.wait(lk, [] {return !data_queue.empty(); }); // 5
	
		data_chunk data = data_queue.front();
		data_queue.pop();
		lk.unlock(); // 6
		process(data);
		if (is_last_chunk(data)) break;
	}
}