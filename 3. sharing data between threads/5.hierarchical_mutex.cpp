#include <mutex>

/*
	���� � ���, ����� ������� ���������� �� �������� ���� � ������� ��� ��������,
	������� ����� ���� ��������� � ������ ����.

	�������� ����� �������� ��������� �������, ���� ��� ��� ����������
	�����-�� ������� ��������������� ����.

	����� ��������� ��� �� ����� ����������, ������� ��������� ������� �������� ����� ����
	� ����� ���� ���������, ����������� ������ �������

	hierarchical_mutex �� �������� � ����������� �����������, �� ����� ���� ����� ���:
*/

class hierarchical_mutex {
private:
	std::mutex internal_mutex;
	unsigned long const hierarchy_value;
	unsigned previous_hierarchy_value;
	static thread_local unsigned long this_thread_hierarchy_value;
	
	void check_for_hierarchy_violation() {
		if (this_thread_hierarchy_value <= hierarchy_value) { // 2
			throw std::logic_error("mutex hierarchy violated");
		}
	}

	void update_hierarchy_value() {
		previous_hierarchy_value = this_thread_hierarchy_value; // 3
		this_thread_hierarchy_value = hierarchy_value;
	}

public:
	explicit hierarchical_mutex(unsigned long value) :
		hierarchy_value(value),
		previous_hierarchy_value(0)
	{};

	void lock() {
		check_for_hierarchy_violation();
		internal_mutex.lock(); // 4
		update_hierarchy_value(); // 5
	}

	void unlock() {
		this_thread_hierarchy_value = previous_hierarchy_value; // 6
		internal_mutex.unlock();
	}

	bool try_lock() {
		check_for_hierarchy_violation();
		if (!internal_mutex.try_lock()) // 7
			return false;
		update_hierarchy_value();
		return true;
	}
};

// ������������� ���������� -> ����������� ��������� ����� �������
// ��� thread_local � ������ ������ - ���� �����
thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);


/* ��� ������� ���� ����� ������������ std::lock_guard<>, �.�  � �� ������������
	��� ��� ������� �����, ����������� ��� �������������� ���������� ��������� ��������:
	lock(), unlock(), try_lock();
*/	

hierarchical_mutex high_level_mutex(10000); 
hierarchical_mutex low_level_mutex(5000); 

int do_low_level_stuff();

int low_level_func() {
	std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
	return do_low_level_stuff();
}

void high_level_stuff(int some_param);

// ����������� high_level_mutex 
// � ����� �������� low_level_func(), � ������� ������������� low_lewel_mutex
// ����� �� ���������.
void high_level_func() {
	std::lock_guard<hierarchical_mutex> lk(high_level_mutex); 
	high_level_stuff(low_level_func()); 
}

// ��������� ������� � ����������� ����������������
void thread_a() { 
	high_level_func();
}

hierarchical_mutex other_mutex(100);
void do_other_stuff();

void other_stuff() {
	high_level_func();
	do_other_stuff();
}

// �������� �������, �� ����� ���������� ��������� � �����������
// �� ���������� ����� ������ � �������� mutex, � ����� �������� ��������� ������ mutex
// --> ���������� ��� ��������� ���������
void thread_b() {
	std::lock_guard<hierarchical_mutex> lk(other_mutex);
		other_stuff();
}