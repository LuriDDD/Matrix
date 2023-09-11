#include "VectorBuf.h"

template<typename T>
class Vector : private VectorBuf<T> {
public:
    using VectorBuf<T>::buf_;
    using VectorBuf<T>::sz_;
    using VectorBuf<T>::used_;

    Vector(Vector&&) noexcept = default;
    Vector& operator=(Vector&&) noexcept = default;

    ~Vector() = default;

    Vector(size_t sz) : VectorBuf<T>(sz) {}

    Vector(const Vector& rhs) : VectorBuf<T>(rhs.used_) {
        while(used_ != sz_) {
            push_back(rhs.buf_[used_]);
        }
    }
    Vector& operator=(const Vector& rhs) {
        Vector tmp(rhs);
        std::swap(*this, tmp);
        return *this;
    }

    void push_back(const T& value) {
        if (used_ != sz_) {
            construct(buf_ + used_, value);
            ++used_;
        }
    }

    void push_back(T&& value) {
        if (used_ != sz_) {
            construct(buf_ + used_, std::move(value));
            ++used_;
        }
    }

};