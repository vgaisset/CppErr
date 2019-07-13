#include <CppErr/Error.h>

void doSomething(cpperr::Error & err) {
    // Equivalent to err.add("An error.")
    err << "An error";
}

int main() {
    // We do not care about the error.
    doSomething(cpperr::_);

    std::cout << cpperr::_;

    return 0;
}

// OUTPUT :
/*
No errors to show.
*/