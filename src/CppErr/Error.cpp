#include <CppErr/Error.h>

#include <sstream>

cpperr::Error cpperr::_;

using namespace cpperr;

const std::vector<ErrorMessage> &Error::stack() const
{
    return stack_;
}

bool Error::hasErrors() const
{
    return !stack_.empty();
}

const ErrorMessage & Error::last() const
{
    return stack_[stack_.size()-1];
}

void Error::add(const ErrorMessage &errorMessage, const std::string &fileName, int line) const
{
    // We do not add anything on the special Error.
    if(this == &_)
        return;

    std::stringstream ss;

    ss << errorMessage.message() << "\nAt " << fileName << ", on line " << line << ". Error Type Id : " << errorMessage.typeId() << ".\n";
    stack_.emplace_back(errorMessage.typeId(), ss.str());
}

void Error::add(const std::string &message, const std::string &fileName, int line) const
{
    add(cpperr::GenericError::defaultError(message), fileName, line);
}

void Error::clear() const
{
    stack_.clear();
}

Error &Error::operator <<(const Error &other)
{
    stack_.insert(stack_.end(), other.stack_.begin(), other.stack_.end());
    return *this;
}

Error::operator bool() const
{
    return hasErrors();
}

std::ostream & cpperr::operator<<(std::ostream &out, const Error &error)
{
    if(error.stack_.empty()) {
        out << "No errors to show.";
        return out;
    }

    out << "=============== STACK TRACE (" << error.stack_.size() << ")\n\n";
    out << "== first -> " << error.stack_[0].message() << "\n";

    for(size_t index = 1; index < error.stack_.size(); ++index)
        out << "== Then  -> " << error.stack_[index].message() << "\n";

    return out << "=============== " << error.stack_.size() << " errors shown\n";
}
