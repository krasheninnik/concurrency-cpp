// The combinable class improves performance by eliminating shared state.
#include <windows.h>
#include <ppl.h>
#include <array>
#include <numeric>
#include <iostream>

using namespace concurrency;
using namespace std;


int main()
{
    array<int, 10> a;
    iota(begin(a), end(a), 0);

    int localSum = 0;
    combinable<int> sum;
    parallel_for_each(begin(a), end(a), [&](int i) {sum.local() += i;});
    localSum = sum.combine(plus<int>());

    cout << localSum << endl;
    return 0;
}