#include <vector>
#include "pch.h"
#include <Spalten/Matrix.hpp>
#include <Spalten/Vector.hpp>

TEST(VectorTests, VectorLvalueConstructor)
{
    std::vector<int> vec1{3, 4, 5, 6, 7};
    auto v0 = Vector<int>(vec1);
    EXPECT_EQ(v0(3), 6);
    EXPECT_EQ(v0(4), 7);
    EXPECT_EQ(v0.dim, 5);
}

TEST(VectorTests, VectorRvalueConstructor)
{
    Vector<int> v1(std::vector<int>{3, 4});
    EXPECT_EQ(v1(0), 3);
    EXPECT_EQ(v1(1), 4);
    EXPECT_EQ(v1.dim, 2);

    Vector<float> v2(std::vector<float>{0.0F});
    EXPECT_THROW(v2(1), std::invalid_argument);
    EXPECT_EQ(v2(0), 0);
    EXPECT_EQ(v2.dim, 1);
}
TEST(VectorTests, VectorDimValueConstructor)
{
    Vector<float> v3(4);
    EXPECT_EQ(v3.dim, 4);
    EXPECT_EQ(v3(0), 0);
    EXPECT_EQ(v3(3), 0);
}
TEST(VectorTests, MagnitudeCalculations)
{
    Vector<int> v1 = Vector<int>(std::vector<int>{3, 4});
    EXPECT_EQ(magnitude(v1.vec), 5.0F);
    auto v2 = Vector<float>(std::vector<float>{-4, 3});
    EXPECT_EQ(5.0F, magnitude(v2.vec));
}

TEST(VectorTests, IteratorCheck)
{
    auto v1 = Vector<int>(std::vector<int>{3, 4, 5});
    int sum = 0;
    for (auto it = v1.begin(); it != v1.end(); ++it)
    {
        sum += *it;
    }
    EXPECT_EQ(sum, 12);

    auto v2 = Vector<int>(std::vector<int>{});
    EXPECT_TRUE(v2.begin() == v2.end());
}

TEST(VectorTests, DimsMatch)
{
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

TEST(VectorTests, VectorEmpty)
{
    auto v1 = Vector<int>(std::vector<int>{});
    EXPECT_EQ(v1.empty(), true);

    auto v2 = Vector<int>(std::vector<int>{3, 4, 5});
    EXPECT_EQ(v2.empty(), false);
}

TEST(VectorTests, CanAccessElements)
{
    Vector<float> v1(std::vector<float>{3, 4, 5});
    EXPECT_EQ(v1(2), 5);
    EXPECT_EQ(v1(0), 3);
    v1(1) = 20;
    EXPECT_EQ(v1(1), 20);
    EXPECT_THROW(v1(4), std::invalid_argument);
    EXPECT_THROW(v1(-1), std::invalid_argument);
}

TEST(VectorTests, ElementWise)
{
    Vector<float> v1(std::vector<float>{3, 4, 5});
    Vector<float> v2(std::vector<float>{2, 3, 4});
    auto v3 = v1 + v2;
    for (int i = 0; i < 3; i++)
    {
        EXPECT_EQ(v3(i), v1(i) + v2(i));
    }
    auto v4 = v1 - v2;
    for (int i = 0; i < 3; i++)
    {
        EXPECT_EQ(v4(i), v1(i) - v2(i));
    }
}
TEST(VectorTests, DotProduct)
{
    Vector<float> v1(std::vector<float>{3, 4});
    Vector<float> v2(std::vector<float>{2, 3});
    EXPECT_EQ(v1 * v2, 18.0F);
    Vector<float> v3(std::vector<float>{-3, 2});
    EXPECT_EQ(v2 * v3, 0.0F);
}

TEST(VectorTests, IncompatibleDimensions)
{
    Vector<float> v1(std::vector<float>{3, 4});
    Vector<float> v2(std::vector<float>{2, 3, 4});
    EXPECT_THROW(v1 + v2, std::invalid_argument);
    EXPECT_THROW(v1 - v2, std::invalid_argument);
    EXPECT_THROW(v1 * v2, std::invalid_argument);
}

TEST(VectorTests, DotProductWithFractionsDetectsIntegerAccumulationBug)
{ // AI
    // This test will fail if Vector::operator* reduces into an integer init (bug).
    Vector<float> a(std::vector<float>{0.5F, 0.5F});
    Vector<float> b(std::vector<float>{0.5F, 0.5F});
    const float expected = 0.5F;
    const float eps = 1e-6F;
    EXPECT_NEAR(a * b, expected, eps);
}