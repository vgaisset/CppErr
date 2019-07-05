#include <CppErr/ErrorMessage.h>

cpperr::ErrorMessage::ErrorMessage() :
    typeId_{cpperr::GenericError::id()},
    message_{"No informations..."}
{
}

cpperr::ErrorMessage::ErrorMessage(int typeId) :
    typeId_{typeId},
    message_{"No informations..."}
{
}

cpperr::ErrorMessage::ErrorMessage(const std::string &message) :
    typeId_{cpperr::GenericError::id()},
    message_{message}
{
}

cpperr::ErrorMessage::ErrorMessage(int typeId, const std::string &message) :
    typeId_{typeId},
    message_{message}
{
}

int cpperr::ErrorMessage::typeId() const
{
    return typeId_;
}

const std::string &cpperr::ErrorMessage::message()
{
    return message_;
}
