#include <CppErr/Result.h>

#include <sstream>

cpperr::Exception::Exception() {
    Error err;
    err << ResultIsSuccessfulError::defaultError();

    std::stringstream ss;
    ss << err;

    message_ = ss.str();
}

cpperr::Exception::Exception(const Error &error)
{
    std::stringstream ss;
    ss << error;

    message_ = ss.str();
}

const char *cpperr::Exception::what() const noexcept
{
    return message_.c_str();
}
