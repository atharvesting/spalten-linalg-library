#pragma once
#include <iostream> // cout, endl
#include <vector> // vector
#include <algorithm> // transform
#include "utils.h"
#include <cmath>
#include <iomanip> // set_precision
#include <set> // set
#include <numeric> // reduce, transform_reduce
#include <execution> // par_unseq, par
#include <functional> // function
#include <utility>
#include <thread> // jthread

/// @brief A class representing a mathematical vector.
/// @tparam T The datatype of the vector elements.
template <typename T>
class Vector {
public:
	std::vector<T> vec;
	size_t dim;

	/// @brief Construct a vector from a std::vector.
	/// @param rray The std::vector to initialize the vector with.
	Vector(const std::vector<T>& rray)
		: vec(rray), dim(vec.size()) {}

	/// @brief Construct a vector by moving a std::vector.
	/// @param rray The std::vector to initialize the vector with.
	Vector(std::vector<T>&& rray) 
		: vec(std::move(rray)), dim(vec.size()) {}

	/// @brief Construct a vector with a specified dimension.
	/// @param dim The dimension of the vector.
	Vector(size_t dim)
		: dim(dim), vec(dim, T{ 0 }) { }

	Vector(size_t dim, T val)
		: dim(dim), vec(dim, T{ val }) {
	}

	using iterator = typename std::vector<T>::iterator;
	using const_iterator = typename std::vector<T>::const_iterator;

	/// @brief Get an iterator to the beginning of the vector.
	/// @return An iterator to the beginning of the vector.
	iterator begin() { return vec.begin(); }

	/// @brief Get an iterator to the end of the vector.
	/// @return An iterator to the end of the vector.
	iterator end() { return vec.end(); }

	/// @brief Get a const iterator to the beginning of the vector.
	/// @return A const iterator to the beginning of the vector.
	const_iterator begin() const { return vec.begin(); }

	/// @brief Get a const iterator to the end of the vector.
	/// @return A const iterator to the end of the vector.
	const_iterator end() const { return vec.end(); }

	/// @brief Check if the dimensions of this vector match those of another vector.
	/// @param other The other vector to compare dimensions with.
	/// @return True if the dimensions match, false otherwise.
	bool dims_match(const Vector& other) const {
		return this->dim == other.dim;
	}

	bool empty() const {
		return this->vec.begin() == this->vec.end();
	}

	/// @brief Get a mutable reference to the element at a specific index.
	/// @param idx The index of the element to retrieve.
	/// @return A mutable reference to the element at the specified index.
	T& operator()(size_t idx) {
		if (0 > idx || idx >= this->dim)
			throw std::invalid_argument("Vector index out of range.");
		return vec[idx];
	}
	
	/// @brief Get the element at a specific index.
	/// @param idx The index of the element to retrieve.
	/// @return The element at the specified index.
	const T& operator()(size_t idx) const {
		if (0 > idx || idx >= this->dim) 
			throw std::invalid_argument("Vector index out of range.");
		return vec[idx];
	}

	/// @brief Perform an element-wise operation on this vector and another vector.
	/// @param other The other vector to perform the operation with.
	/// @param func The function to apply element-wise.
	/// @return The result of the element-wise operation.
	Vector<T> element_wise(const Vector& other, std::function<T(T, T)> func) const {
		if (!dims_match(other)) 
			throw std::invalid_argument("Vector dimensions must match.");
		Vector<T> result(this->dim);
		std::transform(vec.begin(), vec.end(), other.begin(), result.begin(), func);
		return result;
	}

	/// @brief Add another vector to this vector.
	/// @param other The other vector to add.
	/// @return The result of the addition.
	Vector<T> operator+(const Vector& other) const {
		return element_wise(other, std::plus<T>());
	}

	/// @brief Subtract another vector from this vector.
	/// @param other The other vector to subtract.
	/// @return The result of the subtraction.
	Vector<T> operator-(const Vector& other) const {
		return element_wise(other, std::minus<T>());
	}

	/// @brief Calculate the dot product of this vector and another vector.
	/// @param other The other vector to calculate the dot product with.
	/// @return The dot product of the two vectors.
	T operator*(const Vector& other) const {
		if (!dims_match(other)) 
			throw std::invalid_argument("Vector dimensions must match.");
		return std::transform_reduce(vec.begin(), vec.end(), other.begin(), 0, std::plus<T>(), std::multiplies<T>());
	}

	/// @brief Multiply this vector by a scalar.
	/// @param scalar The scalar to multiply by.
	/// @return The result of the multiplication.
	Vector<T> operator*(const float& scalar) const {
		Vector<T> result(this->dim);
		std::transform(std::execution::par_unseq,
			vec.begin(),
			vec.end(),
			result.begin(), [&](T val) { return val * scalar; }
		);
		return result;
	}

	/// @brief Divide this vector by a scalar.
	/// @tparam U The datatype of the scalar.
	/// @param divisor The scalar to divide by.
	/// @return The result of the division.
	template <typename U>
	Vector<float> operator/(const U& divisor) const {
		if (divisor == 0) throw std::invalid_argument("Divisor must not be Zero.");
		Vector<float> result(this->dim);
		std::transform(std::execution::par_unseq, 
			vec.begin(), 
			vec.end(), 
			result.begin(), 
			[&](T val) { 
				return static_cast<float>(val) / static_cast<float>(divisor); 
			}
		);
		return result;
	}

	/// @brief Normalize this vector.
	/// @return The normalized vector.
	Vector<float> normalize() const {
		auto mag = magnitude(*this);
		if (mag == 0) throw std::invalid_argument("Cannot normalize a Zero-Vector.");
		return (*this) / mag;
	}

	/// @brief Calculate the scalar projection of this vector onto another vector.
	/// @param other The other vector to project onto.
	/// @return The scalar projection of this vector onto the other vector.
	float scalar_proj_on(const Vector& other) {
		return (*this * other) / magnitude(other);
	}

	/// @brief Calculate the vector projection of this vector onto another vector.
	/// @param other The other vector to project onto.
	/// @return The Vector object of the projection of this vector onto the other vector.
	Vector<float> vector_proj_on(const Vector& other) {
		return other.normalize() * scalar_proj_on(other);
	}

	/// @brief Calculate the angle between this vector and another vector.
	/// @param other The other vector to calculate the angle with.
	/// @return The angle between the two vectors in radians.
	float angle_between(const Vector& other) {
		return std::acos(((*this) * other) / (magnitude(*this) * magnitude(other)));
	}

	/// @brief Calculate the cosine similarity between this vector and another vector.
	/// @param other The other vector to calculate the cosine similarity with.
	/// @return The cosine similarity between the two vectors within range [-1, 1].
	float cosine_similarity(const Vector& other) {
		return ((*this) * other) / (magnitude(*this) * magnitude(other));
	}

	/// @brief Convert this vector to a Vector of floats.
	/// @return The converted vector.
	Vector<float> to_float() const {
		std::vector<float> converted(vec.begin(), vec.end());
		return Vector<float>(std::move(converted));
	}


};

/// @brief Calculate the magnitude of a vector.
/// @param arr The std::vector for which to calculate the magnitude.
/// @return The magnitude of the vector.
template <typename T>
float magnitude(const std::vector<T>& arr) {
	auto square_sum = std::reduce(
		std::execution::par_unseq,
		arr.begin(),
		arr.end(),
		T{ 0 },
		[](T total, T num) { return total + num * num; }
	);
	return static_cast<float>(std::sqrt(square_sum));
}

/// @brief Calculate the magnitude of a Vector.
/// @param vector The vector object for which to calculate the magnitude.
/// @return The magnitude of the vector.
template <typename T>
float magnitude(const Vector<T>& vector) {
	return magnitude(vector.vec);
}

/// @brief Perform the Gram-Schmidt process on a set of vectors.
/// @param input_vectors The vectors to orthogonalize.
/// @return An std::vector containing the orthogonal basis vectors.
template <typename T>
std::vector<Vector<float>> gram_schmidt(std::vector<Vector<T>>& input_vectors)
{
	std::vector<Vector<float>> ortho_basis;
	if (input_vectors.empty()) return ortho_basis;

	ortho_basis.push_back(input_vectors[0].normalize());

	for (size_t i = 1; i < input_vectors.size(); i++)
	{
		Vector<float> v = input_vectors[i].to_float();
		for (const auto& basis : ortho_basis)
		{
			Vector<float> projection = input_vectors[i].to_float().vector_proj_on(basis);
			v = v - projection;
		}
		ortho_basis.push_back(v.normalize());
	}
	return ortho_basis;
}

/// @brief Perform the Gram-Schmidt process on a set of vectors.
/// @tparam ...Args The types of the input vectors.
/// @param ...args The vectors to orthogonalize.
/// @return An std::vector containing the orthogonal basis vectors.
template <typename T, typename... Args>
std::vector<Vector<float>> gram_schmidt(Args&&... args)
{
	std::vector<Vector<T>> input_vectors{ std::forward<Args>(args)... };
	std::vector<Vector<float>> ortho_basis;
	if (input_vectors.empty()) return ortho_basis;

	ortho_basis.push_back(input_vectors[0].normalize());

	for (size_t i = 1; i < input_vectors.size(); i++)
	{
		Vector<float> v = input_vectors[i].to_float();
		for (const auto& basis : ortho_basis)
		{
			Vector<float> projection = input_vectors[i].to_float().vector_proj_on(basis);
			v = v - projection;
		}
		ortho_basis.push_back(v.normalize());
	}
	return ortho_basis;
}

/// @brief Print the elements of a vector to the console, each on a new line, followed by an empty line for separation.
/// @tparam T The datatype of the vector elements.
/// @param vector The vector object to print.
template <typename T>
void printVec(const Vector<T> vector) {
	for (size_t i = 0; i < vector.dim; i++) {
		std::cout << vector(i) << "\n";
	}
	std::cout << std::endl;
}

/// @brief A class representing a matrix.
/// @tparam T The datatype of the matrix elements.
template <typename T>
class Matrix {
public:
	size_t rows;
	size_t cols;
	std::vector<T> rix;

	/// @brief Construct a matrix with the specified dimensions. Filled with default values of the specified datatype.
	/// @param r The number of rows.
	/// @param c The number of columns.
	Matrix(size_t r, size_t c)
		: rows(r), cols(c), rix(r * c, T{}) {
	}

	/// @brief Construct a square matrix with the specified dimension. Filled with default values of the specified datatype.
	/// @param dim The dimension of the square matrix.
	Matrix(size_t dim)
		: rows(dim), cols(dim), rix(dim * dim, T{}) {
	}

	/// @brief Construct a matrix with the specified dimensions and fill value.
	/// @param r The number of rows.
	/// @param c The number of columns.
	/// @param fill The value to fill the matrix with.
	Matrix(size_t r, size_t c, const T& fill)
		: rows(r), cols(c), rix(r * c, fill) {
	}

	/// @brief Construct a matrix with the specified dimensions and a list of values.
	/// @param r The number of rows.
	/// @param c The number of columns.
	/// @param nums The list of values to initialize the matrix with.
	Matrix(size_t r, size_t c, std::vector<T>&& nums)
		: rows(r), cols(c), rix(std::move(nums)) {
		if (rix.size() != rows * cols)
			throw std::invalid_argument("Inexact amount of numbers provided.");
	}

	template <typename U>
	Matrix(const Matrix<U>& other) : rows(other.rows), cols(other.cols), rix(other.rows * other.cols, T{}) {
		std::transform(
			std::execution::par_unseq,
			other.rix.begin(), other.rix.end(),
			this->rix.begin(), 
			[](const U& val) { return static_cast<T>(val); }
		);
	}

	/// @brief Check if the matrix is square.
	/// @return The dimension of the square matrix if it is square, otherwise 0.
	int is_square() const {
		if (this->rows == this->cols) 
			return this->rows;
		return 0;
	}

	/// @brief Generate an rXc matrix filled with zeros from the appropriate datatype.
	/// @tparam T The datatype of the matrix elements.
	/// @param r The number of rows in the matrix.
	/// @param c The number of columns in the matrix.
	/// @return A matrix filled with zeros.
	static Matrix<T> zeros(size_t r, size_t c) {
		return Matrix<T>(r, c, T{ 0 });
	}

	/// @brief Generate an nXn matrix filled with zeros from the appropriate datatype.
	/// @tparam T The datatype of the matrix elements
	/// @param dim The square dimension of the matrix.
	/// @return An nXn matrix filled with zeros
	static Matrix<T> zeros(size_t dim) {
		return Matrix<T>(dim, dim, T{ 0 });
	}

	/// @brief Generate an rXc matrix filled with ones from the appropriate datatype.
	/// @tparam T The datatype of the matrix elements.
	/// @param r The number of rows in the matrix.
	/// @param c The number of columns in the matrix.
	/// @return A matrix filled with ones.
	static Matrix<T> ones(const size_t& r, const size_t& c) {
		return Matrix<T>(r, c, T{ 1 });
	}

	/// @brief Generate an nXn identity matrix.
	/// @tparam T The datatype of the matrix elements.
	/// @param dim The dimension of the identity matrix.
	/// @return An identity matrix of the specified dimension.
	static Matrix<T> identity(const size_t& dim) {
		auto id = Matrix<T>::zeros(dim, dim);
		for (int i = 0; i < dim; i++)
			id(i, i) = T{ 1 };
		return id;
	}

	/// @brief Fill the matrix with zeros.
	void fill_zeros() {
		std::fill(rix.begin(), rix.end(), static_cast<T>(0));
	}

	/// @brief Fill the matrix with random integer values based on specified range[start, stop).
	/// @param low The lower bound of the range.
	/// @param high The upper bound of the range.
	void fill_random_int(int low, int high) {
		for (auto& num : rix) {
			num = static_cast<T>(generate_random(low, high, false));
		}
	}

	/// @brief Fill the matrix with random float values between [0, 1).
	void fill_random() {
		for (auto& num : rix) {
			num = static_cast<double>(generate_random(0, 0, true));
		}
	}

	/// @brief Fill the matrix with ones.
	void fill_ones() {
		std::fill(rix.begin(), rix.end(), static_cast<T>(1));
	}

	/// @brief Access the element at the specified row and column. Note that the matrix is stored in row-major order, so the index is calculated as r * cols + c.
	/// @tparam T The datatype of the matrix elements.
	/// @param r The row index.
	/// @param c The column index.
	/// @return A const reference to the element at the specified position.
	const T& operator()(size_t r, size_t c) const {
		return rix[r * cols + c];
	}

	/// @brief Access the element at the specified row and column for modification. Note that the matrix is stored in row-major order, so the index is calculated as r * cols + c.
	/// @tparam T The datatype of the matrix elements.
	/// @param r The row index.
	/// @param c The column index.
	/// @return A reference to the element at the specified position.
	T& operator()(size_t r, size_t c) {
		return rix[r * cols + c];
	}

	/// @brief Check if the dimensions of this matrix are equal to those of another matrix.
	/// @param other The other matrix to compare with.
	/// @return True if the dimensions are equal, false otherwise.
	bool dims_equal(const Matrix<T>& other) const {
		return this->rows == other.rows && this->cols == other.cols;
	}

	/// @brief Check if this matrix is equal to another matrix by comparing their dimensions and corresponding elements.
	/// @param other The other matrix to compare with.
	/// @return True if the matrices are equal, false otherwise.
	bool operator==(const Matrix<T>& other) const {
		if (dims_equal(other)) {
			for (int i = 0; i < this->rix.size(); i++) {
				if (this->rix[i] != other.rix[i]) return false;
			}
			return true;
		}
		return false;
	}

	/// @brief Add another matrix to this matrix.
	/// @param other The other matrix to add.
	/// @return A new matrix containing the sum.
	Matrix<T> operator+(const Matrix<T>& other) const {
		if (dims_equal(other)) {
			Matrix<T> result(rows, cols);
			for (size_t i = 0; i < rix.size(); i++)
				result.rix[i] = rix[i] + other.rix[i];
			return result;
		}
		else {
			throw std::invalid_argument("Dimensions of matrices must match for addition.");
		}
	}

	/// @brief Subtract another matrix from this matrix.
	/// @param other The other matrix to subtract.
	/// @return A new matrix containing the difference.
	Matrix<T> operator-(const Matrix<T>& other) const {
		if (dims_equal(other)) {
			Matrix<T> result(rows, cols);
			for (size_t i = 0; i < rix.size(); i++)
				result.rix[i] = rix[i] - other.rix[i];
			return result;
		}
		else {
			throw std::invalid_argument("Dimensions of matrices must match for subtraction.");
		}
	}

	/// @brief Multiply this matrix by another matrix.
	/// @tparam U The datatype of the other matrix elements.
	/// @param other The other matrix to multiply with.
	/// @return A new matrix containing the product.
	template <typename U>
	Matrix<T> operator*(const Matrix<U>& other) const {
		if (this->cols != other.rows)
			throw std::invalid_argument("Column count of first and Row count of second must match for matrix multiplication.");

		Matrix<T> result(this->rows, other.cols);
		for (int i = 0; i < this->rows; i++) {
			for (int j = 0; j < other.cols; j++) {
				for (int k = 0; k < this->cols; k++) {
					result(i, j) += (*this)(i, k) * other(k, j);
				}
			}
		}
		return result;
	}

	/// @brief Copy the elements of another matrix into this matrix if their dimensions match.
	/// @param other The other matrix to copy from.
	void copy(Matrix<T>& other) {
		if (this->rows == other.rows && this->cols == other.cols) {
			this->rix = other.rix;
		}
	}

	/// @brief Multiply the matrix by a scalar value.
	/// @param n The scalar value to multiply by.
	/// @return A new matrix containing the product.
	Matrix<float> operator*(const float scalar) const {
		Matrix<float> result(this->rows, this->cols);
		std::transform(std::execution::par_unseq, 
			this->rix.begin(), this->rix.end(),
			result.rix.begin(), [scalar](T val) { return val * scalar; }
		);
		return result;
	}

	/// @brief Negate all elements of the matrix.
	/// @return New matrix with all elements negated.
	Matrix<T> operator-() const {
		return (*this) * -1;
	}

	template <typename U>
	Matrix<T> fast_mult(const Matrix<U>& other) const {
		if (this->cols != other.rows)
			throw std::invalid_argument("Column count of first and Row count of second must match for matrix multiplication.");

		auto thread_count = std::thread::hardware_concurrency();
		float row_thread_ratio = this->rows / thread_count;
		int chunks = row_thread_ratio > 4 ? static_cast<int>(row_thread_ratio) : 4;

		Matrix<T> result(this->rows, other.cols);
		std::vector<std::jthread> workers;

		for (unsigned int t = 0; t < thread_count; t++) {
			size_t start_row = t * chunks;
			size_t end_row = std::min(start_row + chunks, this->rows);

			if (start_row < end_row) {
				workers.emplace_back([&, start_row, end_row]() {
					for (size_t r = start_row; r < end_row; r++) {
						for (size_t c = 0; c < other.cols; c++) {
							T res{};
							for (size_t k = 0; k < this->cols; k++) {
								res += (*this)(r, k) * other(k, c);
							}
							result(r, c) = res;
						}
					}
					});
			}
		}
		return result;
	}
};


/// @brief Unrefinedly print the matrix to the console with 4 decimal places of precision, along with its dimensions.
/// @tparam T The datatype of the matrix elements.
/// @param mat The matrix to print.
template <typename T>
void printMatrix(const Matrix<T>& mat) {
	std::cout << std::endl << mat.rows << "x" << mat.cols << std::endl;
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			std::cout << std::fixed << std::setprecision(4) << mat(i, j) << "  ";
		}
		std::cout << std::endl;
	}
}

/// @brief Generate an rXc matrix with random float values between [0, 1).
/// @param r The number of rows in the matrix.
/// @param c The number of columns in the matrix.
/// @return A matrix filled with random float values.
Matrix<float> mat_random_uniform(size_t r, size_t c) {
	Matrix<float> mat(r, c);
	for (auto& num : mat.rix) {
		num = static_cast<float>(generate_random(0, 0, true));
	}
	return mat;
}

Matrix<float> mat_random_normal(size_t r, size_t c, float mean=0, float variance=1) {
	Matrix<float> mat(r, c);
	for (auto& num : mat.rix) {
		num = generate_random_n(mean, variance);
	}
	return mat;
}

/// @brief Generate an rXc matrix with random integer values based on specified range[start, stop).
/// @tparam T The datatype of the matrix elements, which should be an integral type.
/// @param r The number of rows in the matrix.
/// @param c The number of columns in the matrix.
/// @param start The lower bound of the random value range.
/// @param stop The upper bound of the random value range.
/// @return A matrix filled with random integer values.
Matrix<int> mat_random_int_range(size_t r, size_t c, int start, int stop) {
	if (start > stop) 
		throw std::invalid_argument("Start must be greater than Stop");
	Matrix<int> mat(r, c);
	for (auto& num : mat.rix) {
		num = static_cast<int>(generate_random(start, stop, false));
	}
	return mat;
}

/// @brief Calculate the trace of a square matrix, which is the sum of its diagonal elements.
/// @tparam T The datatype of the matrix elements.
/// @param mat The square matrix for which to calculate the trace.
/// @return The trace of the matrix.
template <typename T>
T trace(Matrix<T>& mat) {
	if (mat.is_square() == 0) throw std::invalid_argument("Matrix must be a square matrix.");
	T total{ 0 };
	for (int i = 0; i < mat.rows; i++) {
		total += mat(i, i);
	}
	return total;
}

template <typename T>
Matrix<T> transpose(Matrix<T>& mat) {
	Matrix<T> result(mat.cols, mat.rows);
	for (int c = 0; c < result.cols; c++) {
		for (int r = 0; r < result.rows; r++) {
			result(r, c) = mat(c, r);
		}
	}
	return result;
}

/// @brief Struct that holds the L and U matrices resulting from LU decomposition of a square matrix.
struct LU {
	Matrix<float> L;
	Matrix<float> U;

	template <typename T>
	LU(Matrix<T> og, size_t dim) : L(Matrix<float>::identity(dim)), U(og) {
		if (og.rows != dim || og.cols != dim)
			throw std::invalid_argument("Mismatch between specified square dimension and input matrix dimensions.");
	}

	void print() {
		std::cout << "L ";
		printMatrix(L);
		std::cout << "U ";
		printMatrix(U);
	}
};

/// @brief Perform LU decomposition on a square matrix.
/// @tparam T The datatype of the matrix elements.
/// @param mat The square matrix to decompose.
/// @return The LU decomposition structure.
template <typename T>
LU LU_decomp(const Matrix<T>& mat) {
	if (mat.is_square() == 0)
		throw std::invalid_argument("Matrix must be a square matrix.");

	LU lu(mat, mat.rows);

	// For each pivot element in U
	for (size_t p = 0; p < mat.cols - 1; p++)
	{
		float pivot = lu.U(p, p);
		if (pivot == 0.0f)
			throw std::runtime_error("Zero pivot error.");

		// For each row in U under Pivot
		for (size_t r = p + 1; r < mat.rows; r++)
		{
			float multiplier = lu.U(r, p) / pivot;
			lu.L(r, p) = multiplier;

			// For each element in row
			for (size_t c = p; c < mat.cols; c++)
			{
				lu.U(r, c) -= multiplier * lu.U(p, c);
			}
		}
	}
	return lu;
}

/// @brief Solve system of equations using forward substitution, given a lower triangular matrix L and a vector b.
/// @tparam T The datatype of the matrix elements.
/// @tparam U The datatype of the vector elements.
/// @param L The lower triangular coefficient matrix.
/// @param b The vector.
/// @return The solution vector y.
template <typename T, typename U>
Vector<float> fwd_substitution(const Matrix<T>& L, const Vector<U>& b) {
	if (L.rows != b.dim)
		throw std::invalid_argument("Matrix and Vector dimensions must be compatible.");

	if (L.is_square() == 0)
		throw std::invalid_argument("Insufficient data to solve simultaneous equations.");

	Vector<float> y(L.rows);

	for (int i = 0; i < L.rows; i++) {
		float sum = 0.0f;

		for (int j = 0; j < i; j++) {
			sum += y(j) * L(i, j);
		}
		y(i) = static_cast<float>(b(i) - sum) / static_cast<float>(L(i, i));
	}
	return y;
}


/// @brief Calculate the product of all diagonal elements
/// @tparam T The datatype of the matrix
/// @param mat The square matrix for which to find the product of diagonals
/// @return Product of diagonals
template <typename T>
T diag_prod(const Matrix<T>& mat) {
	if (mat.is_square() == 0)
		throw std::invalid_argument("Matrix must be a square matrix.");

	T product = T{ 1 };
	for (int i = 0; i < mat.rows; i++) {
		product *= mat(i, i);
	}
	return product;
}

/// @brief Solve system of equations using backward substitution, given an upper triangular matrix U and a vector y.
/// @tparam T The datatype of the matrix elements.
/// @tparam U_type The datatype of the vector elements.
/// @param U The upper triangular coefficient matrix.
/// @param y The vector.
/// @return The solution vector x.
template <typename T, typename U_type>
Vector<float> bwd_substitution(const Matrix<T>& U, const Vector<U_type>& y) {
	if (U.rows != y.dim)
		throw std::invalid_argument("Matrix and Vector dimensions must be compatible.");

	if (U.is_square() == 0)
		throw std::invalid_argument("Insufficient data to solve simultaneous equations.");

	Vector<float> x(U.rows);

	for (int row = U.rows - 1; row >= 0; row--) {
		float sum = 0.0f;

		for (int col = row + 1; col < U.cols; col++) {
			sum += U(row, col) * x(col);
		}

		x(row) = static_cast<float>(y(row) - sum) / static_cast<float>(U(row, row));
	}
	return x;
}

/// @brief Find matrix determinant using Laplace expansion by minors. 
/// Note that this method is inefficient for large matrices and is intended for educational purposes. 
/// For larger matrices, consider using more efficient algorithms like LU decomposition or SVD.
/// @tparam T The datatype of the matrix elements.
/// @param mat The matrix for which to calculate the determinant.
/// @return The determinant of the matrix.
template <typename T>
int det(const Matrix<T>& mat) {
	if (mat.is_square() == 0) throw std::invalid_argument("Matrix must be a square matrix.");
	if (mat.rows == 1) return mat(0, 0);
	if (mat.rows == 2) return mat(0, 0) * mat(1, 1) - mat(1, 0) * mat(0, 1);

	if (mat.rows < 5) {
		int result = 0;
		Matrix<T> tmp(mat.rows - 1, mat.cols - 1);

		for (int i = 0; i < mat.cols; i++)
		{
			tmp = submatrix(mat, 0, i);
			result += det(tmp) * (std::pow(-1, i) * mat.rix[i]);
		}
		return result;
	}
	else {
		auto lu = LU_decomp(mat);
		return diag_prod(lu.U);
	}
}

/// @brief Find the cofactor matrix of a square matrix, 
/// where each element is the determinant of the corresponding minor multiplied by (-1)^(i+j) 
/// to account for the checkerboard pattern of signs in the cofactor expansion.
/// @tparam T The datatype of the matrix elements.
/// @param mat The square matrix for which to find the cofactor matrix.
/// @return The cofactor matrix.
template <typename T>
Matrix<T> cofactor(Matrix<T>& mat) {
	if (mat.is_square() == 0) throw std::invalid_argument("Matrix must be a square matrix.");

	Matrix<T> result(mat.rows, mat.cols);

	if (mat.rows == 2) {
		result(0, 0) = mat(1, 1); result(0, 1) = -mat(1, 0);
		result(1, 0) = -mat(0, 1); result(1, 1) = mat(0, 0);
		return result;
	}

	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			auto sub = submatrix(mat, i, j);
			result(i, j) = det(sub) * std::pow(-1, i + j);
		}
	}
	return result;
}

/// @brief Find the adjugate of a square matrix, which is the transpose of its cofactor matrix.
/// @tparam T The datatype of the matrix elements.
/// @param mat The square matrix for which to find the adjugate.
/// @return The adjugate of the matrix.
template <typename T>
Matrix<T> adj(Matrix<T>& mat) {
	auto cof = cofactor(mat);
	return transpose(cof);
}

/// @brief Convert a matrix to an std::vector of Vector objects.
/// @tparam T The datatype of the matrix elements.
/// @param mat The matrix to convert.
/// @return An std::vector of Vector objects representing the matrix.
template <typename T>
std::vector<Vector<T>> mat_to_vecs(const Matrix<T>& mat) {
	std::vector<T> tmp; tmp.reserve(mat.rows);
	std::vector<Vector<T>> result;

	for (size_t col = 0; col < mat.cols; col++)
	{
		for (size_t row = 0; row < mat.rows; row++)
		{
			tmp.push_back(mat(row, col));
		}
		result.push_back(Vector<T>(tmp));
		tmp.clear();
	}
	return result;
}

/// @brief Convert an std::vector of Vector objects to a matrix, where each Vector represents a column of the resulting matrix.
/// @tparam T The datatype of the vector elements.
/// @param vecs The vector of Vector objects to convert.
/// @return The resulting matrix.
template <typename T>
Matrix<T> vecs_to_mat(std::vector<Vector<T>> vecs) {
	std::vector<T> combined;
	auto dim = vecs[0].dim;
	auto cols = dim;
	auto rows = vecs.size();

	for (const auto& vec : vecs) {
		if (vec.dim != dim)
			throw std::invalid_argument("All vectors must have the same size.");

		combined.append_range(vec);
	}

	Matrix<T> result(rows, cols, std::move(combined));
	return transpose(result);
}

/// @brief Find the inverse of a square matrix using LU decomposition and forward/backward substitution.
/// @tparam T The datatype of the matrix elements.
/// @param mat The square matrix for which to find the inverse.
/// @return The inverse of the matrix.
template <typename T>
Matrix<float> inv(Matrix<T>& mat) {
	if (mat.is_square() == 0)
		throw std::invalid_argument("Matrix must be a square matrix.");

	/* FINDING INVERSE OF MATRIX A
	* We have A = LU and A x inv(A) = I, then LU * inv(A) = I
	* We take y = U * inv(A), then Ly = I
	* We can split y into n column vectors y1, y2,..., y_n. Same can be done for I - e1, e2,..., e_n
	* This gives us n systems of simultaneous equations that we can solve to find y1, y2,..., y_n
	*	L * y1 = e1, L * y2 = e2,..., L * yn = e_n
	* After finding the matrix y, we have U * inv(A) = y
	* Again, we can split inv(A) into i1, i2,..., i_n. Same can be done for y - y1, y2,..., y_n
	* This gives us n systems of simultaneous equations that we can solve to find i1, i2,..., i_n
	*	which are the n column vectors that make up the inv(A) matrix
	*/

	auto lu = LU_decomp(mat);

	// FORWARD SUBSTITUTION

	auto y_column_vecs = std::vector<Vector<float>>();
	y_column_vecs.reserve(mat.rows);
	auto id_column_vecs = mat_to_vecs(Matrix<float>::identity(mat.rows));

	for (int i = 0; i < mat.rows; i++)
		y_column_vecs.push_back(fwd_substitution(lu.L, id_column_vecs[i]));

	// BACKWARD SUBSTITUTION

	auto inv_column_vecs = std::vector<Vector<float>>();
	inv_column_vecs.reserve(mat.rows);

	for (int i = 0; i < mat.rows; i++)
		inv_column_vecs.push_back(bwd_substitution(lu.U, y_column_vecs[i]));

	return vecs_to_mat(inv_column_vecs);
}

/// @brief Perform the Gram-Schmidt process on a matrix to produce an orthonormal basis.
/// @tparam T The datatype of the matrix elements.
/// @param mat The matrix on which to perform the Gram-Schmidt process.
/// @return The matrix with orthonormal columns.
template <typename T>
Matrix<float> gram_schmidt_mat(const Matrix<T>& mat) {
	std::vector<Vector<T>> vees = mat_to_vecs(mat);
	auto ortho_basis = Vector<T>::gram_schmidt(vees);
	return vecs_to_mat(ortho_basis);
}

/// @brief Perform QR decomposition on a matrix.
/// @tparam T The datatype of the matrix elements.
/// @param A The matrix to decompose.
/// @return A pair containing the Q and R matrices.
template <typename T>
std::pair<Matrix<float>, Matrix<float>> qr_decomp(const Matrix<T>& A) {
	auto Q = gram_schmidt_mat(A);
	auto R = (-Q) * A;
	for (size_t row = 0; row < R.rows; row++)
		for (size_t col = 0; col < R.cols; col++)
			if (col < row)
				R(row, col) = 0;

	std::pair<Matrix<float>, Matrix<float>> qr(Q, R);
	return qr;
}

/// @brief Check if the columns of a matrix are linearly independent.
/// @tparam T The datatype of the matrix elements.
/// @param mat The matrix to check.
/// @return True if the columns are linearly independent, false otherwise.
template <typename T>
bool is_lin_independent(const Matrix<T>& mat) {
	return det(mat);
}

/// @brief Extract a submatrix by excluding a specific row and column.
/// @tparam T The datatype of the matrix elements.
/// @param mat The original matrix.
/// @param exclude_row The index of the row to exclude.
/// @param exclude_col The index of the column to exclude.
/// @return The resulting submatrix.
template <typename T>
Matrix<T> submatrix(const Matrix<T>& mat, int exclude_row, int exclude_col) {
	Matrix<T> result(mat.rows - 1, mat.cols - 1);
	result.rix.clear();

	for (int i = 0; i < mat.rows; i++) {
		if (i == exclude_row) continue;

		for (int j = 0; j < mat.cols; j++) {
			if (j == exclude_col) continue;

			result.rix.push_back(mat(i, j));
		}
	}
	return result;
}

/// @brief Extract a submatrix by excluding multiple rows and columns specified in sets.
/// @tparam T The datatype of the matrix elements.
/// @param mat The original matrix.
/// @param exclude_rows The set of row indices to exclude.
/// @param exclude_cols The set of column indices to exclude.
/// @return The resulting submatrix.
template <typename T>
Matrix<T> submatrix(const Matrix<T>& mat, std::set<int> exclude_rows, std::set<int> exclude_cols) {

	if (exclude_rows.size() > mat.rows || exclude_cols.size() > mat.cols)
		throw std::invalid_argument("Exclusion set size exceeded matrix dimensions.");

	Matrix<T> result(mat.rows - exclude_rows.size(), mat.cols - exclude_cols.size());
	result.rix.clear();

	for (int i = 0; i < mat.rows; i++) {
		if (exclude_rows.find(i) != exclude_rows.end()) continue;

		for (int j = 0; j < mat.cols; j++) {
			if (exclude_cols.find(j) != exclude_cols.end()) continue;

			result.rix.push_back(mat(i, j));
		}
	}
	return result;
}