#include <mutex>

// защита разделяемых данных во время инициальзации
/* вместо того чтобы захватить мьютекс и явно проверять указатель, каждый поток
может вызывать std::call_once. (издержки обычно ниже, чем при применение mutex)

*/
struct some_resource {
	void do_something();
};

std::shared_ptr<some_resource> resource_ptr;
std::once_flag resource_flag;

void init_resource() {
	resource_ptr.reset(new some_resource);
}

void foo() {
	// единственная инициализация
	std::call_once(resource_flag, init_resource);
	// использование
	resource_ptr->do_something();
}

// Можно использовать и для отложенной инициализации членов класса:
struct data_packet {};
struct connection_info {};
struct connection_handle {
	void send_data(data_packet const& d) {};
	data_packet receive_data() {};
};
struct connection_manager_class {
	connection_handle open(connection_info) { return connection_handle(); };
};


connection_manager_class connection_manager;

class X {
private:
	connection_info connection_details;
	connection_handle connection;
	std::once_flag connection_init_flag;

	void open_connection() {
		connection = connection_manager.open(connection_details);
	}

public:
	X(connection_info const& connection_details_) :
		connection_details(connection_details_)
	{};

	void send_data(data_packet const& data) {
		std::call_once(connection_init_flag, &X::open_connection, this);
		connection.send_data(data);
	}

	data_packet receive_data() {
		std::call_once(connection_init_flag, &X::open_connection, this);
		return connection.receive_data();
	}
};