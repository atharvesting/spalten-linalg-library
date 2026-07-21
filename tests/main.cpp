#include <iostream>
#include <vector>
#include <cmath>
#include <thread>

#include <Spalten/Matrix.hpp>
#include <Spalten/Vector.hpp>
#include <Spalten/Utils.hpp>

int main() {
    std::cout << "Benchmarking Matrix Multiplication: Fast vs Naive" << std::endl;
    Timer t, g;
    volatile float dummy_sink = 0.0f;

    for (int i = 5; i <= 1200; i += 25) {

        auto m1 = mat_random_int_range(i, i, 1, 9);
        auto m2 = mat_random_int_range(i, i, 1, 9);

        auto warmup = m1.fast_mult(m2);

        dummy_sink = warmup.rix[0];

        double sum_naive = 0.0, sum_multi = 0.0;
        int repeats = (i < 200) ? 10 : 3;

        for (int r = 0; r < repeats; r++) {
            g.reset();
            auto fast = m1.fast_mult(m2);
            sum_multi += g.elapsed();
            dummy_sink = fast.rix[0];
        }

        for (int r = 0; r < repeats; r++) {
            t.reset();
            auto slow = m1.naive_mult(m2);
            sum_naive += t.elapsed();
            dummy_sink = slow.rix[0];
        }

        std::cout << i << ", " 
                  << sum_multi / repeats << ", " 
                  << sum_naive / repeats << std::endl;
    }

    return 0;
}