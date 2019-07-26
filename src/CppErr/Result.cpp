#include <CppErr/Result.h>

#include <sstream>

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
