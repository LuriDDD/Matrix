
namespace {
    template<typename T>
    void construct(T* p, T&& rhs) {
        new (p) T{std::move(rhs)};
    }

    template<typename T>
    void construct(T* p, const T& rhs) {
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

    explicit VectorBuf(size_t sz = 0) : buf_{(sz == 0) ?
                                                nullptr :
                                                static_cast<T*>(::operator new(sz * sizeof(T)))}, sz_{sz} {};

    VectorBuf(const VectorBuf& rhs) = delete;
    VectorBuf& operator=(const VectorBuf& rhs) = delete;

    VectorBuf(VectorBuf&& rhs) noexcept : buf_{rhs.buf_}, sz_{rhs.sz_}, used_{rhs.used_} {
        rhs.buf_ = nullptr;
        rhs.sz_ = rhs.used_ = 0;
    };
    VectorBuf& operator=(VectorBuf&& rhs) noexcept {
        std::swap(buf_, rhs.buf_);
        std::swap(sz_, rhs.sz_);
        std::swap(used_, rhs.used_);
        return *this;
    };
    ~VectorBuf() {
        destroy(buf_, buf_ + used_);
        ::operator delete(buf_);
    };
};