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

		// извещение ожидающего потока (если такой есть)
		data_cond.notify_one(); // 3
	}
}

// стоит учитывать ложные пробуждения (spurious wake)
// когда поток проснулся не от извещения, а произвольно
// поэтому не желательно использовать функцию для проверки с побочными эффектами.ы
void data_processing_thread() {
	while (true) {
		// неободим unique_lock (возможность захвата и освобождения мьютекса)
		std::unique_lock<std::mutex> lk(mut); // 4

		// в wait передаётся объект блокировки (мьютекс) и предикат (условие)
		// захватывается мьютекс
		// если условие return true -> продолжение выполнения
		// иначе -> освобождение мьютекса и переход в состояние ожидания
		data_cond.wait(lk, [] {return !data_queue.empty(); }); // 5
		// когда прийдёт извещение от notify_one(), процесс проснётся, и снова зайдёт в wait() ^

		data_chunk data = data_queue.front();
		data_queue.pop();
		lk.unlock(); // 6
		process(data);
		if (is_last_chunk(data)) break;
	}
}