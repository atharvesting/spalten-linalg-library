#pragma once
#include <random>        // random_device, mt19937, uniform_real_distribution, uniform_int_distribution, normal_distribution
#include <cmath>         // exp, floor, round
#include <sstream>       // ostringstream
#include <chrono>        // steady_clock, duration, time_point, duration_cast

double generate_random(int low, int high, bool zero2one) {
	static std::random_device rd;
	static std::mt19937 rng(rd());
	if (zero2one) {
		std::uniform_real_distribution<double> distr(0.0, 1.0);
		return distr(rng);
	}
	else {
		std::uniform_int_distribution<> distr(low, high);
		return distr(rng);
	}
}

// https://en.cppreference.com/cpp/numeric/random/normal_distribution
double generate_random_n(double mean, double variance) {
	static std::random_device rd;
	static std::mt19937 rng(rd());
	std::normal_distribution<double> distr{ mean, variance };
	return distr(rng);
}

// Source: https://www.learncpp.com/cpp-tutorial/timing-your-code/
class Timer
{
private:
	// Type aliases to make accessing nested type easier
	using Clock = std::chrono::steady_clock;
	using Second = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<Clock> m_beg{ Clock::now() };

public:
	void reset()
	{
		m_beg = Clock::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
	}
};
//
//void benchmark_multiplication(void) {
//	Timer t, g;
//	for (int i = 5; i < 1200; i += 25) {
//		auto m1 = mat_random_int_range(i, i, 1, 9);
//		auto m2 = mat_random_int_range(i, i, 1, 9);
//		float sum_naive = 0.0f, sum_multi = 0.0f;
//		for (int repeats = 0; repeats < 3; repeats++) {
//			t.reset();
//			auto slow = m1 * m2;
//			sum_naive += t.elapsed();
//			g.reset();
//			//auto fast = fast_mult2(m1, m2);
//			sum_multi += g.elapsed();
//		}
//		std::cout << i << "x" << i << ": Naive: " << sum_naive / 3 << ", Multi-threaded: " << sum_multi / 3 << std::endl;
//	}
//}

//template <typename T>
//concept Streamable =
//	requires(std::ostream & os, T a) {
//	os << a;
//};

//template <Streamable T>
//	requires std::is_arithmetic_v<T>
//int width_of(const T& value) {
//	std::ostringstream oss;
//	oss << value;
//	return oss.str().length();
//}

//int countDigits(int n) {
//    if (n < 10) return 1;
//    if (n < 100) return 2;
//    if (n < 1000) return 3;
//    if (n < 10000) return 4;
//    if (n < 100000) return 5;
//    if (n < 1000000) return 6;
//    if (n < 10000000) return 7;
//    if (n < 100000000) return 8;
//    if (n < 1000000000) return 9;
//    return 10; // for 32-bit max
//}
//
//template <typename T>
//int no_of_digits(T num) {
//    int int_part = std::floor(static_cast<double>(num));
//    int deci_part = static_cast<int>(std::round((num - int_part) * 100000));
//
//    while (static_cast<int>(deci_part) % 10 == 0) deci_part = deci_part / 10;
//
//    return countDigits(int_part) + countDigits(static_cast<int>(deci_part));
//}