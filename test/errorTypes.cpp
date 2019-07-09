#include "test.h"

ERR_CLASS(CustomError, {
public:
    static cpperr::ErrorMessage newError(const std::string & message) {
        return cpperr::ErrorMessage(id(), "My custom message : " + message);
    }
};)

ERR_DECL_NS(err, AnError)
ERR_DECL_NS(othererr, AnError)

int main() {
    Error err;

    ERR_ADD(err, GenericError::defaultError("A generic error."));
    ERR_ADD(err, CustomError::defaultError("A custom error."));
    ERR_ADD(err, CustomError::newError("Test"));
    ERR_ADD(err, "A generic error.");

    const auto& stack = err.stack();

    test(stack[0].is<GenericError>(), "The first error is supposed to be a GenericError.");
    test(!stack[0].is<CustomError>(), "The first error is not supposed to be a CustomError.");

    test(stack[1].is<CustomError>(), "The second error is supposed to be a CustomError.");
    test(!stack[1].is<GenericError>(), "The second error is not supposed to be a GenericError.");

    test(stack[2].message().find("My custom message : Test") != std::string::npos, "A custom message is supposed to be printed.");

    test(othererr::AnError::id() != err::AnError::id(), "Two ErrorTypes of same name but different namespace have same id.");

    test(stack[3].is<cpperr::GenericError>(), "The fourth error is supposed to be a GenericError.");

    std::cout << err;

    return 0;
}
