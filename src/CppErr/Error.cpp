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

void Error::add(const std::string &message, int typeId, const std::string &fileName, int line) const
{
    // We do not add anything on the special Error.
    if(this == &_)
        return;

    std::stringstream ss;

    ss << message << "\nAt " << fileName << ", on line " << line << ". Error Type Id : " << typeId << ".\n";
    stack_.emplace_back(typeId, ss.str());
}

void Error::clear() const
{
    stack_.clear();
}

Error::operator bool() const
{
    return hasErrors();
}

std::ostream & operator<<(std::ostream &out, const Error &error)
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
