#include <iostream>
#include <vector>
#include <string>
#include <string>
#include <chrono>
#include <random>
#include <list>

#include "Vector.h"


struct Y {
    int value;
    Y(int a = 0) : value{a} { }
    Y(const Y& rhs) : value{rhs.value} { }
    Y& operator=(const Y& rhs) { value = rhs.value; return *this;}
    Y(Y&& rhs) noexcept : value{rhs.value} { rhs.value = 0; }
    Y& operator=(Y&& rhs) noexcept { value = rhs.value; rhs.value = 0; return *this;}
    auto operator<=>(const Y&) const = default;
    ~Y() {}
};

struct X {
    X() { std::cout << "X()" << std::endl;}
    X(const X&) { std::cout << "X(&)" << std::endl;}
    X& operator=(const X&) { std::cout << "=X(&)" << std::endl; return *this; }
    X(X&&) { std::cout << "X(&&)" << std::endl; }
    X& operator=(X&&) { std::cout << "=X(&&)" << std::endl; return *this;}
    ~X() { std::cout << "~X()" << std::endl; }
};


auto main() -> decltype(1) {
    const size_t sz = 100000;
    std::random_device rnd_device;
    std::mt19937 mersenne_engine {rnd_device()};
    std::uniform_int_distribution<int> dist {1, 100};
    auto gen = [&dist, &mersenne_engine](){
                   return dist(mersenne_engine);
               };
    auto start_time = std::chrono::high_resolution_clock::now();
    {
        Vector<Y> a (sz);
        Vector<Y> b  = a;
        std::list<Y> l = {0, 1, 2 ,3 ,4 ,5};
        Vector c(l.begin(), l.end());
        std::cout << c.size() << std::endl;
        c[3] = 1;
        auto p = std::mismatch(c.begin(), c.end(), l.begin());
        if (p.first != c.end()) std::cout << p.first->value << std::endl;

        // Vector<Y> v;
        // for (int i = 0; i < sz; ++i) v.push_back(Y{});
        // std::iota(v.begin(), v.end(), 0);
        // std::generate(v.begin(), v.end(), gen);
        // std::shuffle(v.begin(), v.end(), mersenne_engine);
        // std::sort(v.begin(), v.end());
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    {
        std::vector<int> a (sz, 2);
        std::vector<int> b  = a;
        std::vector<int> c(b.begin(), b.end());
        // std::vector<Y> v;
        // for (int i = 0; i < sz; ++i) v.push_back(Y{});
        // std::iota(v.begin(), v.end(), 0);
        // std::generate(v.begin(), v.end(), gen);
        // std::shuffle(v.begin(), v.end(), mersenne_engine);
        // std::sort(v.begin(), v.end());
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;

    // std::for_each(v.begin(), v.end(), [&] (const int& a) { std::cout << a << " ";});
    // std::cout << std::endl;
    return 0;
}