#include "pch.h"
#include "../Spalten/matrix.h"
#include <vector>

TEST(VectorTests, VectorLvalueConstructor) {
    std::vector<int> vec1{ 3, 4, 5, 6, 7 };
    auto v0 = Vector<int>(vec1);
    EXPECT_EQ(v0(3), 6);
    EXPECT_EQ(v0(4), 7);
    EXPECT_EQ(v0.dim, 5);
}

TEST(VectorTests, VectorRvalueConstructor) {
    Vector<int> v1(std::vector<int>{3, 4});
    EXPECT_EQ(v1(0), 3);
    EXPECT_EQ(v1(1), 4);
    EXPECT_EQ(v1.dim, 2);

    Vector<float> v2(std::vector<float>{0.0f});
    EXPECT_THROW(v2(1), std::invalid_argument);
    EXPECT_EQ(v2(0), 0);
    EXPECT_EQ(v2.dim, 1);
}
TEST(VectorTests, VectorDimValueConstructor) {
    Vector<float> v3(4);
    EXPECT_EQ(v3.dim, 4);
    EXPECT_EQ(v3(0), 0);
    EXPECT_EQ(v3(3), 0);
}
TEST(VectorTests, MagnitudeCalculations) {
    Vector<int> v1 = Vector<int>(std::vector<int>{3, 4});
    EXPECT_EQ(magnitude(v1.vec), 5.0f);
    auto v2 = Vector<float>(std::vector<float>{-4, 3});
    EXPECT_EQ(5.0f, magnitude(v2.vec));
}

TEST(VectorTests, IteratorCheck) {
	auto v1 = Vector<int>(std::vector<int>{3, 4, 5});
	int sum = 0;
	for (auto it = v1.begin(); it != v1.end(); ++it) {
		sum += *it;
	}
	EXPECT_EQ(sum, 12);

    auto v2 = Vector<int>(std::vector<int>{});
    EXPECT_TRUE(v2.begin() == v2.end());
}

TEST(VectorTests, DimsMatch) {
    Vector<int> v1(3, 5); // 3d vector filled with 5s
    Vector<int> v2(3, 7); // 3d vector filled with 7s
    EXPECT_TRUE(v1.dims_match(v2));

    Vector<int> v3(4, 5);
    EXPECT_FALSE(v2.dims_match(v3));

    Vector<int> v4(std::vector<int>{});
    EXPECT_FALSE(v3.dims_match(v4));

    Vector<int> v5(std::vector<int>{});
    EXPECT_TRUE(v4.dims_match(v5));
}

TEST(VectorTests, VectorEmpty) {
    auto v1 = Vector<int>(std::vector<int>{});
    EXPECT_EQ(v1.empty(), true);

    auto v2 = Vector<int>(std::vector<int>{3, 4, 5});
    EXPECT_EQ(v2.empty(), false);
}

TEST(VectorTests, CanAccessElements) {
    Vector<float> v1(std::vector<float>{3, 4, 5});
    EXPECT_EQ(v1(2), 5);
    EXPECT_EQ(v1(0), 3);
    v1(1) = 20;
    EXPECT_EQ(v1(1), 20);
    EXPECT_THROW(v1(4), std::invalid_argument);
    EXPECT_THROW(v1(-1), std::invalid_argument);
}

TEST(VectorTests, ElementWise) {
    Vector<float> v1(std::vector<float>{3, 4, 5});
    Vector<float> v2(std::vector<float>{2, 3, 4});
    auto v3 = v1 + v2;
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(v3(i), v1(i) + v2(i));
    }
    auto v4 = v1 - v2;
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(v4(i), v1(i) - v2(i));
    }
}
TEST(VectorTests, DotProduct) {
    Vector<float> v1(std::vector<float>{3, 4});
    Vector<float> v2(std::vector<float>{2, 3});
    EXPECT_EQ(v1 * v2, 18.0f);
    Vector<float> v3(std::vector<float>{-3, 2});
    EXPECT_EQ(v2 * v3, 0.0f);
}

TEST(VectorTests, IncompatibleDimensions) {
	Vector<float> v1(std::vector<float>{3, 4});
	Vector<float> v2(std::vector<float>{2, 3, 4});
	EXPECT_THROW(v1 + v2, std::invalid_argument);
	EXPECT_THROW(v1 - v2, std::invalid_argument);
	EXPECT_THROW(v1 * v2, std::invalid_argument);
}

TEST(MatrixTests, CanFillAndAccessElements) {
    Matrix<int> mat(2, 3);
    mat.fill_ones();

    EXPECT_EQ(2u, mat.rows);
    EXPECT_EQ(3u, mat.cols);
    EXPECT_EQ(1, mat(0, 0));
    EXPECT_EQ(1, mat(1, 2));
}

TEST(MatrixTests, AdditionAddsElementWise) {
    Matrix<int> left(2, 2, 1);
    Matrix<int> right(2, 2, 2);

    auto result = left + right;

    EXPECT_EQ(3, result(0, 0));
    EXPECT_EQ(3, result(0, 1));
    EXPECT_EQ(3, result(1, 0));
    EXPECT_EQ(3, result(1, 1));
}

TEST(MatrixTests, MultiplyMultipliesMatrices) {
    Matrix<int> left(2, 3);
    Matrix<int> right(3, 2);

    left(0, 0) = 1; left(0, 1) = 2; left(0, 2) = 3;
    left(1, 0) = 4; left(1, 1) = 5; left(1, 2) = 6;

    right(0, 0) = 7; right(0, 1) = 8;
    right(1, 0) = 9; right(1, 1) = 10;
    right(2, 0) = 11; right(2, 1) = 12;

    auto result = left * right;

    EXPECT_EQ(2u, result.rows);
    EXPECT_EQ(2u, result.cols);
    EXPECT_EQ(58, result(0, 0));
    EXPECT_EQ(64, result(0, 1));
    EXPECT_EQ(139, result(1, 0));
    EXPECT_EQ(154, result(1, 1));
}

TEST(MatrixTests, ZeroDeterminantForFlatMatrix) {
    Matrix<int> zeros(3, 3);
    EXPECT_EQ(0, det(zeros));

    Matrix<int> ones(4, 4);
    EXPECT_EQ(0, det(ones));
}

TEST(MatrixTests, IncompatibleDimensiions) {
    Matrix<int> ones(2, 4, 1);
    Matrix<int> twos(3, 3, 2);

    EXPECT_THROW(ones + twos, std::invalid_argument);
    EXPECT_THROW(twos + ones, std::invalid_argument);
    EXPECT_THROW(ones - twos, std::invalid_argument);
    EXPECT_THROW(twos - ones, std::invalid_argument);
    EXPECT_THROW(ones * twos, std::invalid_argument);
    EXPECT_THROW(twos * ones, std::invalid_argument);
    EXPECT_THROW(det(ones), std::invalid_argument);
    EXPECT_THROW(adj(ones), std::invalid_argument);
    EXPECT_THROW(inv(ones), std::invalid_argument);
}

TEST(MatrixTests, DimsEqual) {
    Matrix<int> m(3, 2);
    Matrix<int> b(3, 2);
    Matrix<int> c(2, 1);

    EXPECT_EQ(m.dims_equal(b), true);
    EXPECT_EQ(m.dims_equal(c), false);
    EXPECT_EQ(b.dims_equal(m), true);
    EXPECT_EQ(c.dims_equal(m), false);
}