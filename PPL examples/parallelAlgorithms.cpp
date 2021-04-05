#include <ppl.h>
#include <array>
#include <sstream>
#include <iostream>
#include <vector>

using namespace concurrency;
using namespace std;

template <typename T>
T twice(const T& t) {
    return t + t;
}

int main()
{
    cout << "Parallel for: " << endl;
    parallel_for(1, 6, [](int value) { cout << value << " ";  });

    cout << endl;
    cout << "Parallel for_each: " << endl;
    array<int, 5> values = { 1, 2, 3, 4, 5 };
    parallel_for_each(begin(values), end(values), [](int value) {cout << value << " ";  });

    cout << endl;
    cout << "Parallel invoke:" << endl;
    int n = 101;
    string s = "Hello";
    parallel_invoke(
        [&n] {n = twice(n); },
        [&s] {s = twice(s); }
    );
    cout << "Twiced values: " << n << " and " << s << endl;

    cout << endl;
    cout << "Parallel transform:" << endl;
    vector<int> v1{ 1,2,3,4,5,6,7,8 };
    parallel_transform(v1.begin(), v1.end(), v1.begin(), [](int n) {return n * n; });
    for_each(v1.begin(), v1.end(), [](int n) {cout << n << " "; });

    cout << endl;
    cout << "Binary parallel transform:" << endl;
    vector<int> v2{ 1,2,3,4,5,6,7,8,9 };
    vector<int> v3(v2.rbegin(), v2.rend());
    vector<int> result(v2.size());;
    parallel_transform(v2.begin(), v2.end(), v3.begin(), result.begin(), plus<int>());
    for_each(result.begin(), result.end(), [](int n) {cout << n << " "; });

    cout << endl;
    cout << "Parallel reduce:" << endl;
    cout << parallel_reduce(result.begin(), result.end(), int()) << endl;
}