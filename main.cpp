#include <iostream>

#include "VectorBuf.h"

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
    return 0;
}