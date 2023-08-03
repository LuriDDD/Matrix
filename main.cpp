#include <iostream>
#include <iterator>


template<typename T>
class MatrixBuf {
protected:
    T* matrix_;
    int n, m;

    MatrixBuf(int n, int m, T t = T{}) 
    : matrix_(new T[n][m]) {}
    MatrixBuf(const MatrixBuf& rhs) {

    }
    MatrixBuf& opearator=(const MatrixBuf& rhs);
    MatrixBuf(MatrixBuf&& rhs) noexcept 
    : matrix_(rhs.matrix_) {
        rhs.matrix_ = nullptr;
    }
    MatrixBuf& opearator=(MatrixBuf&& rhs) noexcept {
        std::swap(matrix_, rhs.matrix_);
        return *this;
    }
    ~MatrixBuf() {
        delete[] matrix_;
    }
};


int main() {
    return 0;
}