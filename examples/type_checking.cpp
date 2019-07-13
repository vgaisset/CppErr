#include <CppErr/Error.h>
#include <cmath>

// Declares classes representing error type.
// Those classes can be implemented in order to provide custom ErrorMessages.
ERR_DECL(DivisionByZeroError);

namespace err {
ERR_CLASS_NS(err, NegativeNumberError, 
public:
    static cpperr::ErrorMessage newError(int number) {
        return defaultError("A positive value is expected (value = " + std::to_string(number) + ").");
    }
);
}

// Computes sqrt(a) / b.
double squareDivide(int a, double b, cpperr::Error & err) { // cpperr::Err is equivalent to const cpperr:Err &.
    if(a < 0) {
        ERR_ADD(err, err::NegativeNumberError::newError(a));
        return -1;
    } else if (b == 0) {
        ERR_ADD(err, DivisionByZeroError::defaultError());
        return -1;
    }
    return std::sqrt(a) / b;
}

int main() {
    cpperr::Error err;

    squareDivide(-2, 4, err);

    if(err.is<err::NegativeNumberError>()) {
        std::cout << "Negative number" << "\n";
    } else if (err.is<DivisionByZeroError>()) {
        std::cout << "Division by zero" << "\n";
    }

    // OR

    squareDivide(4, 0, err);

    if(err) { // First check if there is at least an error message.
        switch(err.last().typeId()) {
            case err::NegativeNumberError::id() :
                std::cout << "Negative number" << "\n";
                break;
            case DivisionByZeroError::id() :
                std::cout << "Division by zero" << "\n";
                break;
        }
    }

    std::cout << err;

    return 0;
}

// OUTPUT :
/*
Negative number
Division by zero
=============== STACK TRACE (2)

=(1)= first -> NegativeNumberError from namespace err (id = -1508031621)
=== At type_checking.cpp, on line 20
A positive value is expected (value = -2).

=(2)= Then  -> DivisionByZeroError (id = -842168546)
=== At type_checking.cpp, on line 23

=============== 2 errors shown
*/