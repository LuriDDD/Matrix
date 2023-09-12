#include "VectorBuf.h"

#include <algorithm>
#include <iostream>
#include <exception>
#include <stdexcept>

template<typename T>
class Vector : private VectorBuf<T> {
public:
    using VectorBuf<T>::buf_;
    using VectorBuf<T>::sz_;
    using VectorBuf<T>::used_;

    Vector(Vector&&) noexcept = default;
    Vector& operator=(Vector&&) noexcept = default;

    ~Vector() = default;

    Vector(size_t sz = 0) : VectorBuf<T>(sz) {
        while(used_ != sz_) {
            push_back();
        }
    }

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

    void push_back() {
        if (used_ == sz_) {
            Vector<T> tmp = copy_and_swap(sz_ * 2);
            tmp.push_back();
            *this = std::move(tmp);
            return;
        }
        construct(buf_ + used_);
        ++used_;
    }

    void push_back(const T& value) {
        if (used_ == sz_) {
            Vector<T> tmp = copy_and_swap(sz_ * 2);
            tmp.push_back(value);
            *this = std::move(tmp);
            return;
        }
        construct(buf_ + used_, value);
        ++used_;
    }

    void push_back(T&& value) {
        if (used_ == sz_) {
            Vector<T> tmp = copy_and_swap(sz_ * 2);
            tmp.push_back(std::move(value));
            *this = std::move(tmp);
            return;
        }
        construct(buf_ + used_, std::move(value));
        ++used_;
    }

    Vector<T> copy_and_swap(size_t sz) {
        Vector<T> tmp(((sz) ? sz : 2), true);
        std::for_each(tmp.buf_, tmp.buf_ + used_, [&] (T& a) { construct(std::addressof(a), buf_[tmp.used_]); ++tmp.used_;});
        return tmp;
    }

    void reserve(size_t sz) {
        if (sz > sz_) {
            Vector<T> tmp = copy_and_swap(sz);
            *this = std::move(tmp);
        }
    }

    void clear() {
        destroy(buf_, buf_ + used_);
        used_ = 0;
    }

    size_t capacity() const noexcept { return sz_; }

    size_t size() const noexcept { return used_; }

    bool empty() const noexcept { return !used_; }

    T& operator[](size_t pos) { return *buf_[pos]; }

    const T& operator[](size_t pos) const { return *buf_[pos]; }

    T& front() { return operator[](0); }

    const T& front() const { return operator[](0); }

    T& back() { return operator[](used_ - 1); }

    const T& back() const { return operator[](used_ - 1); }

    T& at(size_t pos) {
        if (pos >= used_ || pos < 0) throw std::out_of_range("out_of_range");
        return operator[](pos); 
    }

    const T& at(size_t pos) const {
        if (pos >= used_ || pos < 0) throw std::out_of_range("out_of_range");
        return operator[](pos);
    }

private:
    Vector(size_t sz, bool) : VectorBuf<T>(sz) {}
};