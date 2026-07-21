#pragma once
#include <vector>        // vector
#include <algorithm>     // transform
#include <numeric>       // reduce, transform_reduce
#include <cmath>         // sqrt, acos
#include <execution>     // execution::par_unseq
#include <iostream>      // cout, endl
#include <stdexcept>     // invalid_argument
#include <utility>       // move, forward
#include <type_traits>   // common_type_t

#include "Utils.hpp"

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
		: vec(rray), dim(vec.size()) {
	}

	/// @brief Construct a vector by moving a std::vector.
	/// @param rray The std::vector to initialize the vector with.
	Vector(std::vector<T>&& rray)
		: vec(std::move(rray)), dim(vec.size()) {
	}

	/// @brief Construct a vector with a specified dimension.
	/// @param dim The dimension of the vector.
	Vector(size_t dim)
		: dim(dim), vec(dim, T{ 0 }) {
	}

	/// @brief Construct a vector with a specified dimension and fill value.
	/// @param dim The dimension of the vector.
	/// @param val The value to fill the vector with.
	Vector(size_t dim, T val)
		: dim(dim), vec(dim, T{ val }) {
	}

	using iterator = std::vector<T>::iterator;
	using const_iterator = std::vector<T>::const_iterator;

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
	bool dims_equal(const Vector& other) const {
		return this->dim == other.dim;
	}

	/// @brief Check if the vector is empty.
	/// @return True if the vector is empty, false otherwise.
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

	/// @brief Check if this vector is equal to another vector.
	/// @param other The other vector to compare with.
	/// @return True if the vectors are equal, false otherwise.
	bool operator==(const Vector<T>& other) const {
		if (!dims_equal(other))
			return false;
		return std::equal(this->begin(), this->end(), other.begin());
	}

	/// @brief Perform an element-wise operation on this vector and another vector.
	/// @param other The other vector to perform the operation with.
	/// @param func The function to apply element-wise.
	/// @return The result of the element-wise operation.
	template <typename U>
	auto _element_wise(const Vector<U>& other, auto func) const {
		if (!dims_equal(other))
			throw std::invalid_argument("Vector dimensions must match.");

		using ResultType = std::common_type_t<T, U>;
		Vector<ResultType> result(this->dim);

		std::transform(std::execution::par_unseq,
			vec.begin(), vec.end(),
			other.begin(), result.begin(),
			func
		);
		return result;
	}

	/// @brief Add another vector to this vector.
	/// @param other The other vector to add.
	/// @return The result of the addition.
	template <typename U>
	auto operator+(const Vector<U>& other) const {
		return _element_wise(other, std::plus<>());
	}

	/// @brief Subtract another vector from this vector.
	/// @param other The other vector to subtract.
	/// @return The result of the subtraction.
	template <typename U>
	auto operator-(const Vector<U>& other) const {
		return _element_wise(other, std::minus<>());
	}

	/// @brief Calculate the dot product of this vector and another vector.
	/// @param other The other vector to calculate the dot product with.
	/// @return The dot product of the two vectors.
	template <typename U>
	auto operator*(const Vector<U>& other) const {
		if (!dims_equal(other))
			throw std::invalid_argument("Vector dimensions must match.");

		return std::transform_reduce(
			std::execution::par_unseq,
			vec.begin(), vec.end(),
			other.begin(), std::common_type_t<T, U>{},
			std::plus<>(), std::multiplies<>()
		);
	}

	/// @brief Multiply this vector by a scalar.
	/// @param scalar The scalar to multiply by.
	/// @return The result of the multiplication.
	template <typename U>
	auto operator*(const U& scalar) const {
		using ResultType = std::common_type_t<T, U>;
		Vector<ResultType> result(this->dim);

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
		[](T total, T num) { return total + (num * num); }
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
	std::cout << "\n";
}