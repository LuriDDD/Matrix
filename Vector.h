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

    class Iterator {
    public:
        using iterator_type = Iterator;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::random_access_iterator_tag;
        using iterator_concept = std::contiguous_iterator_tag;
    private:
        pointer value_;
    public:
        Iterator() noexcept : value_{pointer()} {}

        Iterator(pointer value) noexcept : value_{value} {}

        Iterator(const Iterator& rhs) noexcept : value_{rhs.value_} {}
        Iterator& operator=(const Iterator& rhs) noexcept { value_= rhs.value_; return *this; }

        Iterator(Iterator&& rhs) noexcept : value_{rhs.value_} { rhs.value_ = nullptr; }
        Iterator& operator=(Iterator&& rhs) { value_ = rhs.value_; rhs.value_ = nullptr; return *this; }

        reference operator*() noexcept { return *value_; }

        pointer operator->() noexcept { return value_; }

        Iterator& operator++() noexcept { ++value_; return *this; }

        Iterator& operator--() noexcept { --value_; return *this; }

        Iterator operator++(int) noexcept { return Iterator(value_++); }

        Iterator operator--(int) noexcept { return Iterator(value_--); }

        reference& operator[](int pos) noexcept { return value_[pos]; }

        Iterator& operator+=(int pos) noexcept { value_ += pos; return *this; }

        Iterator& operator-=(int pos) noexcept { value_ -= pos; return *this; }

        Iterator operator+(int pos) const noexcept { return Iterator(value_ + pos); }

        Iterator operator-(int pos) const noexcept { return Iterator(value_ - pos); }

        difference_type operator-(const Iterator& rhs) const noexcept { return value_ - rhs.value_; }

        auto operator<=>(const Iterator&) const = default;
    };

    Iterator begin() const { return Iterator(buf_); }

    Iterator end() const { return Iterator(buf_ + used_); }

private:
    Vector(size_t sz, bool) : VectorBuf<T>(sz) {}
};