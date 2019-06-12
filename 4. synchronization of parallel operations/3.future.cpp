// std::future � std::shared_future ��������� �� ������� 
// std::unique_ptr � std::shared_ptr: �� ���� ������ ����� ���������
// ������ ���� std::future, �� ��������� std::shared_future

#include <future>
#include <iostream>

int find_the_answer_to_lturae();
void do_other_sturff();

// async �� ������ ��������� ������������ ����� �����,
// ��� ������ ������������� ���������� ���������, ����������� �� �������:
// std::launch::deferred - �������� �� get() or wait()
// std::launch::async - ������� ������������ ����� �����
// std::launch::deferred | std::launch::async - �� ���������� ���������� (�� ���������)

int main() {
	// async - ��������� ��������� ����������� ������
	std::future<int> the_answer = std::async(find_the_answer_to_lturae);
	do_other_sturff();

	// std::future::get() -- ������������� ������� ����������, ���� 
	// �� futere �� ����� ���������� ��������. ������ �������� �����������.
	std::cout << "the answer is: " << the_answer.get() << std::endl;
}