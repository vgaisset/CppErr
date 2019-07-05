#pragma once

#include <CppErr/ErrorType.h>

#include <string>

namespace cpperr {

/**
 * @brief
 * -> A class representing an error with its associated message.
 * It also contains the type id of the error, which can be used to know
 * what kind of error it represents.
 */
class ErrorMessage {
private:
    int typeId_;
    std::string message_;

public:
    ErrorMessage();
    ErrorMessage(int typeId);
    ErrorMessage(const std::string & message);
    ErrorMessage(int typeId, const std::string & message);

    /**
     * @brief
     * Returns the type id representing this error.
     * @return
     * The type id.
     */
    int typeId() const;

    /**
     * @brief
     * Returns the message describing the error.
     * @return
     * The message describing the error.
     */
    const std::string & message();
};

}

