#include <CppErr/Error.h> // The only include you need.

void doSomething(cpperr::Error & err) { 
    // Failed to do something.

    // Adds an error for file basic.cpp and at line 7.
    ERR_ADD(err, "Failed to do something.");
}

ERR_CLASS(MyCustomError,
public:
    static cpperr::ErrorMessage newError(int i) {
        return defaultError("My custom error : " + std::to_string(i));
    }
); // Creates an ErrorType names MyCustomError.

int main() {
    cpperr::Error err;

    doSomething(err);

    if(err) {
        std::cout << err << "\n";
        return 1;
    }

    return 0;
}

// OUTPUT :
/*
=============== STACK TRACE (1)

=(1)= first -> GenericError from namespace cpperr (id = -1502749837)
=== At basic.cpp, on line 7
Failed to do something.

=============== 1 errors shown
*/