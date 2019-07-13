#include <CppErr/Error.h>

cpperr::Error doSomething(int i) {
    cpperr::Error err;
    ERR_ADD(err, "Failed to do something " + std::to_string(i));

    return err;
}

int main() {
    cpperr::Error err;

    err << doSomething(0) << doSomething(1);

    if(err) {
        std::cout << err << "\n";
        return 1;
    }

    return 0;
}

// OUTPUT :
/*
=============== STACK TRACE (2)

=(1)= first -> GenericError from namespace cpperr (id = -1502749837)
=== At error_concatenation.cpp, on line 5
Failed to do something 0

=(2)= Then  -> GenericError from namespace cpperr (id = -1502749837)
=== At error_concatenation.cpp, on line 5
Failed to do something 1

=============== 2 errors shown
*/