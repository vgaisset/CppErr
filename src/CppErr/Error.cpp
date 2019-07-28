#include <CppErr/Error.h>

#include <sstream>

cpperr::Error cpperr::_;

using namespace cpperr;

Error::Error(const ErrorMessage &errorMessage, const std::string &fileName, int line)
{
    add(errorMessage, fileName, line);
}

Error::Error(const std::string &message, const std::string &fileName, int line)
{
    add(message, fileName, line);
}

const ErrorStack &Error::stack() const
{
    return stack_;
}

bool Error::hasErrors() const
{
    return !stack_.empty();
}

const LocatedErrorMessage &Error::last() const
{
    return stack_[stack_.size()-1];
}

void Error::add(const ErrorMessage &errorMessage, const std::string &fileName, int line)
{
    // We do not add anything on the special Error.
    if(this == &_)
        return;

    stack_.emplace_back(errorMessage, fileName, line);
}

void Error::add(const std::string &message, const std::string &fileName, int line)
{
    add(cpperr::GenericError::defaultError(message), fileName, line);
}

void Error::clear()
{
    stack_.clear();
}

Error &Error::operator <<(const Error &other)
{
    // Adds other's stack on top of this' stack.
    stack_.insert(stack_.end(), other.stack_.begin(), other.stack_.end());
    return *this;
}

Error &Error::operator <<(const std::string &message)
{
    add(message);
    return *this;
}

Error &Error::operator <<(const ErrorMessage &errorMessage)
{
    add(errorMessage);
    return *this;
}

Error::operator bool() const
{
    return hasErrors();
}

std::ostream & cpperr::operator<<(std::ostream &out, const Error &error)
{
    if(!error.hasErrors()) {
        out << "No errors to show.\n";
        return out;
    }

    out << "=============== STACK TRACE (" << error.stack_.size() << ")\n\n";
    out << "=(1)=" << " first -> " << error.stack_[0] << "\n";

    for(size_t index = 1; index < error.stack_.size(); ++index)
        out << "=(" << std::to_string(index+1) << ")= Then  -> " << error.stack_[index] << "\n";

    return out << "=============== " << error.stack_.size() << " errors shown\n";
}
