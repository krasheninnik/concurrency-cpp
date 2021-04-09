#include <mutex>

std::recursive_mutex rm;

/* recursive_mutex works similarly to mutex, with one difference:
	the same thread can acquire a given mutex multiple times.
	But before this mutex can take over another thread,
	he must be freed as many times as he was captured.
	!!! It is not recommended for use !!!
*/