#pragma once
#include "matrix.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>

int main() {

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
