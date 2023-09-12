#include <iostream>
#include <vector>
#include <string>
#include <string>
#include <chrono>

#include "Vector.h"


struct Y {
    Y() {}
    Y(const Y&) {}
    Y& operator=(const Y&) {return *this;}
    Y(Y&&) {}
    Y& operator=(Y&&) {return *this;}
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
 
int main()
{
    constexpr size_t sz = 5;
    auto start_time = std::chrono::high_resolution_clock::now();
    {
        Vector<Y> a (sz);
        Vector<Y> b  = a;
        Vector<Y> c(std::move(b));
        Vector<Y> s;
        for (int i = 0; i < sz; ++i) s.push_back(Y{});
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;
    return 0;
}