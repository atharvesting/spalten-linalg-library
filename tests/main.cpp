#pragma once
#include <Spalten/Matrix.hpp>
#include <Spalten/Vector.hpp>
#include <Spalten/Utils.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>

int main() {

	Timer t, g;
	for (int i = 5; i < 1200; i += 25) {
		auto m1 = mat_random_int_range(i, i, 1, 9);
		auto m2 = mat_random_int_range(i, i, 1, 9);
		float sum_naive = 0.0f, sum_multi = 0.0f;
		//for (int repeats = 0; repeats < 10; repeats++) {
		//	g.reset();
		//	auto fast = m1.fast_mult(m2);
		//	sum_multi += g.elapsed();
		//}
        for (int repeats = 0; repeats < 10; repeats++) {
            t.reset();
            auto slow = m1 * m2;
            sum_naive += t.elapsed();
        }
		std::cout << i << ", " << sum_naive / 10.0f << std::endl; // ", " << sum_naive / 10.0f <<
	}

    //Timer time_normal;
    //Timer time_fast;

    //for (size_t i = 2; i < 20; i++) 
    //{
    //    auto mat = mat_random_int_range(i, i, 1, 9);
    //    double sum_normal = 0.0, sum_fast = 0.0;

    //    for (int j = 0; j < 10; j++) {
    //        
    //        time_normal.reset();
    //        auto inv_normal = det(mat);
    //        sum_normal += time_normal.elapsed();
    //        
    //        time_fast.reset();
    //        auto inv_f = det_fast(mat);
    //        sum_fast += time_fast.elapsed();
    //        
    //    }
    //    std::cout << i << ": Normal = " << sum_normal / 10.0 << " ";
    //    std::cout << "Fast = " << sum_fast / 10.0 << "\n";
    //}

    //auto m1 = Matrix<int>(3, 3, std::vector<int>{
    //    4, 3, 8,
    //    6, 2, 5,
    //    1, 5, 9
    //});
    //printMatrix(m1);

    //auto inverse = inv_fast(m1);
    //printMatrix(inverse);


    return 0;
}