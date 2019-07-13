#include <CppErr/ErrorMessage.h>
#include <CppErr/ErrorType.h>

#include <iostream>

using namespace cpperr;

std::string ErrorMessage::defaultMessage = "";

cpperr::ErrorMessage::ErrorMessage() :
    typeId_{cpperr::GenericError::id()},
    namespaceName_{cpperr::GenericError::namespaceName},
    typeName_{cpperr::GenericError::typeName},
    message_{defaultMessage}
{
}

cpperr::ErrorMessage::ErrorMessage(int typeId, const char *namespaceName, const char *typeName) :
    typeId_{typeId},
    namespaceName_{namespaceName},
    typeName_{typeName},
    message_{defaultMessage}
{
}

cpperr::ErrorMessage::ErrorMessage(const std::string &message) :
    typeId_{cpperr::GenericError::id()},
    namespaceName_{cpperr::GenericError::namespaceName},
    typeName_{cpperr::GenericError::typeName},
    message_{message}
{
}

ErrorMessage::ErrorMessage(int typeId, const char *namespaceName, const char *typeName, const std::string &message) :
    typeId_{typeId},
    namespaceName_{namespaceName},
    typeName_{typeName},
    message_{message}
{
}

int cpperr::ErrorMessage::typeId() const
{
    return typeId_;
}

const char *ErrorMessage::namespaceName() const
{
    return namespaceName_;
}

const char *ErrorMessage::typeName() const
{
    return typeName_;
}

const std::string &cpperr::ErrorMessage::message() const
{
    return message_;
}

/////////////////////////////////////////
// LocatedErrorMessage implementation. //
/////////////////////////////////////////

std::string LocatedErrorMessage::defaultFileName    = "Unknown file";
int         LocatedErrorMessage::defaultLineNumber  = -1;

LocatedErrorMessage::LocatedErrorMessage(const ErrorMessage &errorMessage, const std::string &fileName, int line) :
    errorMessage_(errorMessage),
    fileName_(fileName),
    line_(line)
{
}

int LocatedErrorMessage::typeId() const
{
    return errorMessage_.typeId();
}

const char *LocatedErrorMessage::namespaceName() const
{
    return errorMessage_.namespaceName();
}

const char *LocatedErrorMessage::typeName() const
{
    return errorMessage_.typeName();
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

std::ostream & cpperr::operator <<(std::ostream &out, const LocatedErrorMessage &errorMessage)
{
    out << errorMessage.typeName();

    if(*errorMessage.namespaceName() != '\0')
        out << " from namespace " << errorMessage.namespaceName();

    out << " (id = " << errorMessage.typeId() << ")";

    out << "\n=== At " << errorMessage.fileName() << ", on line " << errorMessage.line();

    if(!errorMessage.message().empty())
        out << "\n" << errorMessage.message();

    return out << "\n";
}
