# Spalten - Accelerated Linear Algbera Library. Built for constructing Deep Neural Networks.

Spalten is a an educational linear algebra library (MVP done) written in C++. It is easy to use, flexible, and fairly fast. The motivation behind building this library was:
1. To learn linear algebra
2. To serve as the foundational matrix engine for [NNFS-Extreme (Neural Nets from absolute scratch)](https://github.com/atharvesting/nnfs-extreme).

The library leverages standard library execution policies, SIMD-friendly memory layouts (row-major), and parallelized algorithms to provide great performance without the code looking like the mess I leave under my bed (no offense to actually serious libraries).

## Key Features

- **`Matrix<T>` template**: The primary offering of this library. Includes foundational operations (determinant, transpose and inverse) as well as numerical decomposition methods (LU, QR, Gram-Schmidt, back/forward substitutions).

- **`Vector<T>` template**: A vector abstraction featuring foundational functions along with interoperability (still WIP) with `Matrix<T>`.

- **Zero external dependencies**: Built entirely with standard modern C++, keeping compilation straightforward and cross-platform portable.

## Example Usage

```cpp
#include <iostream>
#include "Spalten/Matrix.hpp"

int main() {
    // Instantiate 3x3 floating-point matrices
    auto A = Matrix<float>(3, 3, {1, 2, 3,
                                  6, 4, 2,
                                  9, 2, 1});

    // Compute transpose and determinant
    auto A_transposed = transpose(A);
    float det = det(A);

    // Numerical decomposition
    struct LU lu = lu_decomp(A);
    // Struct accessed using lu.L and lu.U

    std::cout << "Determinant: " << det << "\n";
    return 0;
}
```

```Output
Determinant: -48
```
---

The project is part of a mult-layered and multi-faceted progression, which was conceptualized by me to take a journey through low-level C++, 
ML mathematics, Concurrency, Computer Vision, all the way to resource-efficient-aggresive TinyML.

- [x] Phase 1: [Spalten](https://github.com/atharvesting/spalten-linalg-library) (Foundational Linear Algebra Library)
- [x] Phase 2: [NNFS-Extreme](https://github.com/atharvesting/nnfs-extreme) (Neural Network trained on the MNIST dataset) -> Built on top of Spalten
- [ ] Phase 3: MNIST-CV (CV layer that classifies digits drawn using gestures) -> NNFS-E as the engine
- [ ] Phase 4: ESP32-MNIST (The entire pipeline running on an ESP32-S3 Sense) -> Using all of the above

More about this to be documented soon!

---
