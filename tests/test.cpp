#include <vector>
#include "pch.h"
#include <Spalten/Matrix.hpp>
#include <Spalten/Vector.hpp>

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
    EXPECT_TRUE(v1.dims_equal(v2));

    Vector<int> v3(4, 5);
    EXPECT_FALSE(v2.dims_equal(v3));

    Vector<int> v4(std::vector<int>{});
    EXPECT_FALSE(v3.dims_equal(v4));

    Vector<int> v5(std::vector<int>{});
    EXPECT_TRUE(v4.dims_equal(v5));
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

TEST(VectorTests, DotProductWithFractionsDetectsIntegerAccumulationBug) { // AI
    // This test will fail if Vector::operator* reduces into an integer init (bug).
    Vector<float> a(std::vector<float>{0.5f, 0.5f});
    Vector<float> b(std::vector<float>{0.5f, 0.5f});
    const float expected = 0.5f;
    const float eps = 1e-6f;
    EXPECT_NEAR(a * b, expected, eps);
}

//================================================//

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

TEST(MatrixTests, TransposeContentsAndShape) { // AI
    Matrix<int> m(2, 3);
    int val = 1;
    for (size_t r = 0; r < m.rows; ++r)
        for (size_t c = 0; c < m.cols; ++c)
            m(r, c) = val++;

    auto t = transpose(m);
    EXPECT_EQ(t.rows, 3u);
    EXPECT_EQ(t.cols, 2u);

    // check elements: t(r,c) == m(c,r)
    for (size_t r = 0; r < t.rows; ++r)
        for (size_t c = 0; c < t.cols; ++c)
            EXPECT_EQ(t(r, c), m(c, r));
}

TEST(MatrixTests, InverseRoundTripApproximatelyIdentity) { // AI
    Matrix<float> A(2, 2);
    A(0, 0) = 4.0f; A(0, 1) = 7.0f;
    A(1, 0) = 2.0f; A(1, 1) = 6.0f;

    auto Ainv = inv(A);
    auto prod = A * Ainv;

    const float eps = 1e-4f;
    EXPECT_EQ(prod.rows, 2u);
    EXPECT_EQ(prod.cols, 2u);
    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 2; ++j)
            EXPECT_NEAR(prod(i, j), (i == j ? 1.0f : 0.0f), eps);
}

TEST(MatrixTests, LURecomposeMatchesOriginal) { // AI
    Matrix<float> A(3, 3);
    A(0, 0) = 2; A(0, 1) = 1; A(0, 2) = 1;
    A(1, 0) = 4; A(1, 1) = -6; A(1, 2) = 0;
    A(2, 0) = -2; A(2, 1) = 7; A(2, 2) = 2;

    auto lu = LU_decomp(A); // lu.L and lu.U are float matrices
    auto recomposed = lu.L * lu.U;

    const float eps = 1e-4f;
    EXPECT_EQ(recomposed.rows, A.rows);
    EXPECT_EQ(recomposed.cols, A.cols);
    for (size_t r = 0; r < A.rows; ++r)
        for (size_t c = 0; c < A.cols; ++c)
            EXPECT_NEAR(recomposed(r, c), A(r, c), eps);
}

TEST(MatrixTests, MatToVecsAndBackRoundtrip) { // AI
    Matrix<int> M(3, 2);
    M(0, 0) = 1; M(1, 0) = 2; M(2, 0) = 3;
    M(0, 1) = 4; M(1, 1) = 5; M(2, 1) = 6;

    auto vecs = mat_to_vecs(M);
    auto M2 = vecs_to_mat(vecs);

    EXPECT_EQ(M.rows, M2.rows);
    EXPECT_EQ(M.cols, M2.cols);
    for (size_t r = 0; r < M.rows; ++r)
        for (size_t c = 0; c < M.cols; ++c)
            EXPECT_EQ(M(r, c), M2(r, c));
}



