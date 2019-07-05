#include "test.h"

ERR_DECL(CustomError);

int main() {
    Error err;

    ERR_ADD_T(err, "A generic error.", GenericError);
    ERR_ADD_T(err, "A custom error.", CustomError);

    const auto& stack = err.stack();

    test(stack[0].is<GenericError>(), "The first error is supposed to be a GenericError.");
    test(!stack[0].is<CustomError>(), "The first error is not supposed to be a CustomError.");

    test(stack[1].is<CustomError>(), "The second error is supposed to be a CustomError.");
    test(!stack[1].is<GenericError>(), "The second error is not supposed to be a GenericError.");

    test(err.last().is<CustomError>(), "The last error is supposed to be a CustomError.");
    test(!err.last().is<GenericError>(), "The last error is not supposed to be a GenericError.");

    return 0;
}
