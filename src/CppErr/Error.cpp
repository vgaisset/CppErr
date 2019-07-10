#include <CppErr/Error.h>

#include <sstream>

cpperr::Error cpperr::_;

using namespace cpperr;

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

void Error::add(const ErrorMessage &errorMessage, const std::string &fileName, int line) const
{
    // We do not add anything on the special Error.
    if(this == &_)
        return;

    stack_.emplace_back(errorMessage, fileName, line);
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
    // Adds other's stack on top of this' stack.
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
    out << "== first -> " << error.stack_[0] << "\n";

    for(size_t index = 1; index < error.stack_.size(); ++index)
        out << "== Then  -> " << error.stack_[index] << "\n";

    return out << "=============== " << error.stack_.size() << " errors shown\n";
}

std::ostream & cpperr::operator <<(std::ostream &out, const LocatedErrorMessage &errorMessage)
{
    out << errorMessage.message() << "\nAt "
        << errorMessage.fileName() << ", on line "
        << errorMessage.line() << ". Error Type Id : "
        << errorMessage.typeId() << ".\n";

    return out;
}

LocatedErrorMessage::LocatedErrorMessage(const ErrorMessage &errorMessage, const std::string &fileName, int line) :
    errorMessage_(errorMessage),
    fileName_(fileName),
    line_(line)
{
}

const std::string &LocatedErrorMessage::message() const
{
    return errorMessage_.message();
}

const std::string &LocatedErrorMessage::fileName() const
{
    return fileName_;
}

int LocatedErrorMessage::line() const
{
    return line_;
}

int LocatedErrorMessage::typeId() const
{
    return errorMessage_.typeId();
}
