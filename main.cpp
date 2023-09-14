#include <iostream>
#include <vector>
#include <string>
#include <string>
#include <chrono>
#include <random>

#include "Vector.h"


struct Y {
    int value;
    Y(int a = 0) : value{a} {}
    Y(const Y& rhs) : value{rhs.value} {}
    Y& operator=(const Y& rhs) { value = rhs.value; return *this;}
    Y(Y&& rhs) : value{rhs.value} { rhs.value = 0; }
    Y& operator=(Y&& rhs) { value = rhs.value; rhs.value = 0; return *this;}
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

int main() {
    const size_t sz = 1'000'000;
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
        Vector<Y> c(std::move(b));
        Vector<Y> v;
        for (int i = 0; i < sz; ++i) v.push_back(Y{});
        std::iota(v.begin(), v.end(), 0);
        std::generate(v.begin(), v.end(), gen);
        std::shuffle(v.begin(), v.end(), mersenne_engine);
        std::sort(v.begin(), v.end());
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    {
        std::vector<Y> a (sz);
        std::vector<Y> b  = a;
        std::vector<Y> c(std::move(b));
        std::vector<Y> v;
        for (int i = 0; i < sz; ++i) v.push_back(Y{});
        std::iota(v.begin(), v.end(), 0);
        std::generate(v.begin(), v.end(), gen);
        std::shuffle(v.begin(), v.end(), mersenne_engine);
        std::sort(v.begin(), v.end());
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;

    // std::for_each(v.begin(), v.end(), [&] (const int& a) { std::cout << a << " ";});
    // std::cout << std::endl;
    return 0;
}