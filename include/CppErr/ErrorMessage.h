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
    const std::string & message() const;

    /**
     * @brief
     * Checks if the given ErrorType is the same than the one used when creating this error message.
     * This function works only with type inherited from cpperr::ErrorType<id>.
     * Returns true if the error type is the same than given type, false otherwise.
     * @return
     * True if the last error type is the same than given type, false otherwise.
     */
    template<class TErrorType>
    bool is() const {
        return TErrorType::id() == typeId_;
    }
};

}

