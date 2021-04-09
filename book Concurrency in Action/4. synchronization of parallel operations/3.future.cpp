// std::future and std::shared_future modeled on std::unique_ptr and std::shared_ptr:
// One object can be referenced by only one std::future, but several std::shared_future

#include <future>
#include <iostream>

int find_the_answer_to_lturae();
void do_other_sturff();

// async does not always start an asynchronous thread right away,
// for explicit regulation there is a parameter specified before the function:
// std::launch::deferred - defer untill get() or wait()
// std::launch::async - launc rigth now
// std::launch::deferred | std::launch::async - (default case: discretion of the implementation) 

int main() {
	// async - enable to run async task
	std::future<int> the_answer = std::async(find_the_answer_to_lturae);
	do_other_sturff();

	// std::future::get() -- stop current execution until futere is not calculated
	std::cout << "the answer is: " << the_answer.get() << std::endl;
}