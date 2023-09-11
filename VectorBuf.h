
namespace {
    template<typename T>
    void construct(const T* p, const T& rhs) {
        new (p) T{rhs};
    }

    template<typename T>
    void destroy(T* p) noexcept {
        p->~T();
    }

    template<typename Iter>
    void destroy(Iter begin, Iter end) noexcept {
        for (;begin != end; ++begin) {
            destroy(std::addressof(*begin));
        }
    }
}

template<typename T>
class VectorBuf {
protected:
    T* buf_;
    size_t sz_, used_ = 0;

    VectorBuf() : buf_{nullptr}, sz_{0} {};
    explicit VectorBuf(size_t sz) : buf_{::operator new(sz_ * sizeof(T))}, sz_{sz} {};

    VectorBuf(const VectorBuf& rhs) = delete;
    VectorBuf& operator=(const VectorBuf& rhs) = delete;

    VectorBuf(VectorBuf&& rhs) : buf_{rhs.buf_} {
        std::swap(sz_, rhs.sz_);
        std::swap(used_, rhs.used_);
    };
    VectorBuf& operator=(VectorBuf&& rhs) {
        std::swap(*this, rhs);
    };
    ~VectorBuf() {
        destroy(buf_, buf_ + used_);
        ::operator delete(buf_);
    };
};