// std::future и std::shared_future устроенны по образцу 
// std::unique_ptr и std::shared_ptr: на один объект может ссылатьс€
// только один std::future, но несколько std::shared_future

#include <future>
#include <iostream>

int find_the_answer_to_lturae();
void do_other_sturff();

// async не всегда запускает ассинхронный поток сразу,
// дл€ €вного регулировани€ существует параметер, указываемый до функции:
// std::launch::deferred - отложить до get() or wait()
// std::launch::async - запусть ассинхронный поток сразу
// std::launch::deferred | std::launch::async - на усмотрение реализации (по умолчанию)

int main() {
	// async - позвол€ет запустить асинхронную задачу
	std::future<int> the_answer = std::async(find_the_answer_to_lturae);
	do_other_sturff();

	// std::future::get() -- останавливает текущее исполнение, пока 
	// во futere не будет вычисленно значение. ƒанное значение извлекаетс€.
	std::cout << "the answer is: " << the_answer.get() << std::endl;
}