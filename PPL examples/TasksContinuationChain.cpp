// https://docs.microsoft.com/en-us/cpp/parallel/concrt/task-parallelism-concurrency-runtime?view=msvc-160
#include <ppltasks.h>
#include <iostream>

using namespace concurrency;
using namespace std;

int wmain()
{
    auto t = create_task([]() -> int
        {
            return 0;
        });

    // Create a lambda that increments its input value.
    auto increment = [](int n) { return n + 1; };

    // Run a chain of continuations and print the result.
    int result = t.then(increment).then(increment).then(increment).get();
    wcout << result << endl;
}