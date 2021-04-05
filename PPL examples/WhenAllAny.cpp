// join-tasks.cpp
// compile with: /EHsc
#include <ppltasks.h>
#include <array>
#include <iostream>
#include <numeric>

using namespace concurrency;
using namespace std;

int wmain()
{
    // Start multiple tasks.
    array<task<int>, 3> tasks = {
        create_task([]() -> int { return 88; }),
        create_task([]() -> int { return 42; }),
        create_task([]() -> int { return 99; })
    };

    std::cout << "When all case: " << std::endl;
    auto joinTask = when_all(begin(tasks), end(tasks)).then([](vector<int> results)
        {
            wcout << L"The sum is "
                << accumulate(begin(results), end(results), 0)
                << L'.' << endl;
        });

    // Print a message from the joining thread.
    wcout << L"Hello from the joining thread." << endl;

    // Wait for the tasks to finish.
    joinTask.wait();

    std::cout << std::endl;
    std::cout << "When any case: " << std::endl;
    // Select the first to finish.
    when_any(begin(tasks), end(tasks)).then([](pair<int, size_t> result)
        {
            wcout << "First task to finish returns "
                << result.first
                << L" and has index "
                << result.second
                << L'.' << endl;
        }).wait();
}