#include <vector>
#include "pch.h"
#include <Spalten/Matrix.hpp>

template <typename T>
void container_check(const Matrix<T>& left, const Matrix<T>& right, double tolerance = 1e-6) {
    EXPECT_EQ(left.rows, right.rows);
    EXPECT_EQ(left.cols, right.cols);
    EXPECT_EQ(left.size(), right.size());
    for (size_t i = 0; i < left.size(); i++)
        EXPECT_NEAR(left.rix[i], right.rix[i], tolerance);
}

TEST(MatrixTests, CanFillAndAccessElements)
{
    Matrix<int> mat(2, 3);
    mat.fill_ones();

    EXPECT_EQ(2U, mat.rows);
    EXPECT_EQ(3U, mat.cols);
    EXPECT_EQ(1, mat(0, 0));
    EXPECT_EQ(1, mat(1, 2));
}

TEST(MatrixTests, AdditionAddsElementWise)
{
    Matrix<int> left(2, 2, 1);
    Matrix<int> right(2, 2, 2);

    auto result = left + right;

    EXPECT_EQ(3, result(0, 0));
    EXPECT_EQ(3, result(0, 1));
    EXPECT_EQ(3, result(1, 0));
    EXPECT_EQ(3, result(1, 1));
}

TEST(MatrixTests, MultiplyMultipliesMatrices)
{
    Matrix<int> left(2, 3);
    Matrix<int> right(3, 2);

    left(0, 0) = 1;
    left(0, 1) = 2;
    left(0, 2) = 3;
    left(1, 0) = 4;
    left(1, 1) = 5;
    left(1, 2) = 6;

    right(0, 0) = 7;
    right(0, 1) = 8;
    right(1, 0) = 9;
    right(1, 1) = 10;
    right(2, 0) = 11;
    right(2, 1) = 12;

    auto result = left * right;

    EXPECT_EQ(2U, result.rows);
    EXPECT_EQ(2U, result.cols);
    EXPECT_EQ(58, result(0, 0));
    EXPECT_EQ(64, result(0, 1));
    EXPECT_EQ(139, result(1, 0));
    EXPECT_EQ(154, result(1, 1));
}

TEST(MatrixTests, ZeroDeterminantForFlatMatrix)
{
    Matrix<int> zeros(3, 3);
    EXPECT_EQ(0, det(zeros));

    Matrix<int> ones(4, 4);
    EXPECT_EQ(0, det(ones));
}

TEST(MatrixTests, IncompatibleDimensiions)
{
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

TEST(MatrixTests, Dims_Equal)
{
    Matrix<int> m(3, 2);
    Matrix<int> b(3, 2);
    Matrix<int> c(2, 1);

    EXPECT_EQ(m.dims_equal(b), true);
    EXPECT_EQ(m.dims_equal(c), false);
    EXPECT_EQ(b.dims_equal(m), true);
    EXPECT_EQ(c.dims_equal(m), false);
}

TEST(MatrixTests, TransposeContentsAndShape)
{ // AI
    Matrix<int> m(2, 3);
    int val = 1;
    for (size_t r = 0; r < m.rows; ++r)
        for (size_t c = 0; c < m.cols; ++c)
            m(r, c) = val++;

    auto t = transpose(m);
    EXPECT_EQ(t.rows, 3U);
    EXPECT_EQ(t.cols, 2U);

    // check elements: t(r,c) == m(c,r)
    for (size_t r = 0; r < t.rows; ++r)
        for (size_t c = 0; c < t.cols; ++c)
            EXPECT_EQ(t(r, c), m(c, r));
}

TEST(MatrixTests, TransposeProperties) {
    // (A^T)^T = A
    auto m = mat_random_int_range(5, 4, 1, 10);
    auto m_t = transpose(m);
    auto m_tt = transpose(m_t);
    EXPECT_EQ(m.rows, m_tt.rows);
    EXPECT_EQ(m.cols, m_tt.cols);
    EXPECT_TRUE(m == m_tt);

    // (AB)^T = B^T * A^T
    auto b = mat_random_int_range(4, 3, 1, 10);
    auto b_t = transpose(b);
    auto mb = m * b;
    auto mb_t = transpose(mb);
    container_check(mb_t, (b_t * m_t));

    // (kA)^T = k * A^T
    auto c = m * 4;
    auto c_t = transpose(c);
    container_check(c_t, (m_t * 4));

    // inv(A^T) = inv(A)^T for invertible square matrices
    Matrix<float> sq(2, 2);
    sq(0, 0) = 4.0F;
    sq(0, 1) = 7.0F;
    sq(1, 0) = 2.0F;
    sq(1, 1) = 6.0F;
    container_check(transpose(inv(sq)), inv(transpose(sq)));
}

TEST(MatrixTests, DeterminantProperties) {
    // det(I) = 1
    auto I = Matrix<float>::identity(4);
    EXPECT_EQ(det(I), 1.0F);

    // det(AB) = det(A) * det(B)
    auto A = mat_random_int_range(3, 3, 1, 10);
    auto B = mat_random_int_range(3, 3, 1, 10);
    EXPECT_NEAR(det(A * B), det(A) * det(B), 1e-3F);

    // det(cA = c^n * det(A) for nXn matrix
    auto c = 3.0F;
    auto det_cA = det(A * c);
    auto det_A = det(A);
    auto n = static_cast<float>(A.rows);
    EXPECT_NEAR(det_cA, std::pow(c, n) * det_A, 1e-3F);

    // det(A^T) = det(A)
    EXPECT_NEAR(det(transpose(A)), det(A), 1e-3F);
}

TEST(MatrixTests, CofactorAndAdjugateProperties) {
    // adj(A) = cofactor(A)^T
    auto A = mat_random_int_range(3, 3, 1, 10);
    auto adj_A = adj(A);
    auto cofactor_A = cofactor(A);
    container_check(adj_A, transpose(cofactor_A));

    // adj(I) = I
    auto I = Matrix<float>::identity(4);
    container_check(adj(I), I);

    // adj(cA) = c^(n-1) * adj(A) for nXn matrix
    auto c = 2.0F;
    EXPECT_TRUE( adj(A * c) ==  adj_A * (std::pow(c, A.rows - 1)) );

    // adj(A^T) = adj(A)^T
    EXPECT_TRUE(adj(transpose(A)) == transpose(adj_A));

    // det(adj(A)) = det(A)^(n-1) for nXn matrix
    EXPECT_NEAR(det(adj_A), std::pow(det(A), A.rows - 1), 1e-3F);

    // adj(A) = det(A) * inv(A) for invertible nXn matrix
    if (det(A) != 0.0F) {
        auto inv_A = inv(A);
        for (size_t r = 0; r < A.rows; ++r)
            for (size_t c = 0; c < A.cols; ++c)
                EXPECT_NEAR(adj_A(r, c), det(A) * inv_A(r, c), 1e-3F);
    }
}

TEST(MatrixTests, InverseProperties) {
    // A * inv(A) = inv(A) * A = I
    auto A = mat_random_int_range(3, 3, 1, 10);
    if (det(A) != 0.0F) {   
        auto inv_A = inv(A);
        auto I1 = A * inv_A; 
        auto I2 = inv_A * A;
        auto I = Matrix<float>::identity(A.rows);
        container_check(I1, I);
        container_check(I2, I);
    }

    // inv( inv(A) ) = A
    if (det(A) != 0.0F) {
        auto inv_A = inv(A);
        auto inv_inv_A = inv(inv_A);
        for (size_t r = 0; r < A.rows; ++r)
            for (size_t c = 0; c < A.cols; ++c)
                EXPECT_NEAR(inv_inv_A(r, c), A(r, c), 1e-3F);
    }

    // inv(kA) = (1/k) * inv(A) for non-zero scalar k
    auto k = 3.0F;
    if (det(A) != 0.0F) {
        container_check(inv(A * k), (inv(A) * (1.0F / k)));
    }

    // det(inv(A)) = 1 / det(A) for invertible matrix A
    if (det(A) != 0.0F) {
        EXPECT_NEAR(det(inv(A)), 1.0F / det(A), 1e-3F);
    }
}

TEST(MatrixTests, InverseRoundTripApproximatelyIdentity)
{ // AI
    Matrix<float> A(2, 2);
    A(0, 0) = 4.0F;
    A(0, 1) = 7.0F;
    A(1, 0) = 2.0F;
    A(1, 1) = 6.0F;

    auto Ainv = inv(A);
    auto prod = A * Ainv;

    const float eps = 1e-3;
    EXPECT_EQ(prod.rows, 2U);
    EXPECT_EQ(prod.cols, 2U);
    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 2; ++j)
            EXPECT_NEAR(prod(i, j), (i == j ? 1.0F : 0.0F), eps);
}

TEST(MatrixTests, LUDecompositionProperties) {
    // A = L * U = A
    auto m = mat_random_int_range(3, 3, 1, 10);
    auto lu = LU_decomp(m);
    auto prod = lu.L * lu.U;
    for (size_t r = 0; r < m.rows; ++r)
        for (size_t c = 0; c < m.cols; ++c)
            EXPECT_NEAR(prod(r, c), m(r, c), 1e-3);

    // det(L) = diag_prod(L) = 1 for lower triangular matrix with 1s on diagonal
    EXPECT_NEAR(det(lu.L), 1.0F, 1e-3);

    // det(U) = diag_prod(U) = det(A)
    EXPECT_NEAR(det(lu.U), det(m), 1e-3);
}

TEST(MatrixTests, LURecomposeMatchesOriginal)
{ // AI
    Matrix<float> A(3, 3);
    A(0, 0) = 2;
    A(0, 1) = 1;
    A(0, 2) = 1;
    A(1, 0) = 4;
    A(1, 1) = -6;
    A(1, 2) = 0;
    A(2, 0) = -2;
    A(2, 1) = 7;
    A(2, 2) = 2;

    auto lu = LU_decomp(A); // lu.L and lu.U are float matrices
    auto recomposed = lu.L * lu.U;

    const float eps = 1e-3;
    EXPECT_EQ(recomposed.rows, A.rows);
    EXPECT_EQ(recomposed.cols, A.cols);
    for (size_t r = 0; r < A.rows; ++r)
        for (size_t c = 0; c < A.cols; ++c)
            EXPECT_NEAR(recomposed(r, c), A(r, c), eps);
}

TEST(MatrixTests, MatToVecsAndBackRoundtrip)
{ // AI
    Matrix<int> M(3, 2);
    M(0, 0) = 1;
    M(1, 0) = 2;
    M(2, 0) = 3;
    M(0, 1) = 4;
    M(1, 1) = 5;
    M(2, 1) = 6;

    auto vecs = mat_to_vecs(M);
    auto M2 = vecs_to_mat(vecs);

    EXPECT_EQ(M.rows, M2.rows);
    EXPECT_EQ(M.cols, M2.cols);
    for (size_t r = 0; r < M.rows; ++r)
        for (size_t c = 0; c < M.cols; ++c)
            EXPECT_EQ(M(r, c), M2(r, c));
}