#include <array>
#include <algorithm>
#include <iostream>
#include <chrono>

#define COUNT 100000000
using Array = std::array<float, 512>;
template<typename Arg>
void measure(Arg&& fun)
{
	using namespace std;
	Array arr;
	for(int i = 0; i < arr.size(); i++)
		arr[i] = 1;
	auto start = chrono::steady_clock::now();
	for(int i = 0; i <= COUNT; i++)
		fun(arr);
	auto end = chrono::steady_clock::now();
	auto diff = end - start;
	cout << chrono::duration <double, milli> (diff).count() / COUNT << " ms" << endl;
}

int main()
{
	using namespace std;

	auto l1 = [] (auto&i) { i*=2; };
	auto l2 = [] (auto&i) { i*=3; };

	measure([] (Array& y) { for_each(begin(y), end(y), [] (auto& i) { i *= 2; }); });
	measure([] (Array& y) { transform(begin(y), end(y), begin(y), [] (auto i) { return i * 2; }); });
	measure([] (Array& y) { for(auto& val : y) val *= 2; });
	measure([] (Array& y) { for(int i = 0; i < y.size(); i++) y[i] *= 2; });
	measure([&] (Array& y) { for(int i = 0; i < y.size(); i++) l1(y[i]); });
	measure([&] (Array& y) { for(int i = 0; i < y.size(); i++) {l1(y[i]); l2(y[i]);} });
	measure([] (Array& y) { for(decltype(y.size()) i = 0; i < y.size(); ++i) y[i] *= 2; });

	measure([&] (Array& y) { for_each(begin(y), end(y), [&] (auto& i) { l1(i); l2(i); }); });

	return 0;
}
