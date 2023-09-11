#include <iostream>
#include <vector>

#include "Vector.h"

struct X {
    X() { std::cout << "X()" << std::endl; }
    X(const X&) { std::cout << "X(&)" << std::endl; }
    X& operator=(const X&) { std::cout << "=X(&)" << std::endl; return *this; }
    X(X&&) { std::cout << "X(&&)" << std::endl; }
    X& operator=(X&&) { std::cout << "=X(&&)" << std::endl; return *this;}
    ~X() { std::cout << "~X()" << std::endl; }
};
 
int main()
{
    {
        Vector<X> v(3);
        X x;
        v.push_back(x);
        Vector<X> a(v);
    }
    std::cout << "-------------------------------" << std::endl;
    std::vector<X> v(3);
    X x;
    v.push_back(x);
    std::vector<X> a(v);
    return 0;
}