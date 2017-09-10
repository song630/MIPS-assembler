#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <iostream>
#include <string>
using namespace std;

class Exception
{
public:
    Exception(const string name): error_type(name) {}
    ~Exception() {}
    void print_error() {
        cout << "Error: " << error_type << endl;
    }

private:
    const string error_type;
};

#endif // EXCEPTIONS_H
