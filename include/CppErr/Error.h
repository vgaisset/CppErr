#pragma once

#include <CppErr/ErrorType.h>

#include <iostream>
#include <vector>
#include <type_traits>
#include <memory>

#define ERR_ADD(err, err_message) \
    err.add(err_message, __FILE__, __LINE__)

namespace cpperr {

class Error;

using ErrorStack = std::vector<LocatedErrorMessage>;

/**
 * @brief
 * -> Represents an error. It holds a stack in which new ErrorMessages can
 * be added.
 * -> One macro is available to help writting code :
 * ERR_ADD(err, err_message) : Adds the ErrorMessage to the error and set filename and line.
 * The err_message parameter can also be string and will be considered as a
 * cpperr::GenericError.
 */
class Error {

public:
    ErrorStack stack_;

    Error() = default;

    /**
     * @brief
     * The stack containing LocatedErrorMessages.
     * @return
     * Returns the stack containing LocatedErrorMessages.
     */
    const ErrorStack& stack() const;

    /**
     * @brief
     * Does the error hold at least one ErrorMessage on its stack ?
     * @return
     * True if there are one or more ErrorMessages on the stack, false otherwise.
     */
    bool hasErrors() const;

    /**
     * @brief
     * Returns the last ErrorMessage in the stack.
     * If the stack is empty, a segfault will happen.
     * See hasErrors() to check if the stack is empty or not
     * before calling this function.
     * @return
     * The last ErrorMessage in the stack.
     */
    const LocatedErrorMessage &last() const;

    /**
     * @brief
     * Adds an error to the stack.
     * The macro ERROR(err, msg) can be used to
     * add the message 'msg' to the Error 'err' wtih __FILE__ as fileName and __LINE__ as line.
     * @param errorMessage
     * The ErrorMessage to add to the error.
     * @param typeId
     * The id of the type representing the error.
     * @param fileName
     * The filename of the file where the error occured on.
     * @param line
     * The line number where the error occured.
     */
    void add(const ErrorMessage & errorMessage, const std::string & fileName = LocatedErrorMessage::defaultFileName,
                                                int line = LocatedErrorMessage::defaultLineNumber);

    /**
     * @brief
     * Shortcut for adding an ErrorMessage with no ErrorType.
     * See the other add() function for more informations.
     * @param message
     * The message describing the error.
     * @param typeId
     * The id of the type representing the error.
     * @param fileName
     * The filename of the file where the error occured on.
     * @param line
     * The line number where the error occured.
     */
    void add(const std::string & message, const std::string & fileName = LocatedErrorMessage::defaultFileName,
                                          int line = LocatedErrorMessage::defaultLineNumber);

    /**
     * @brief
     * Clears the stack.
     */
    void clear();

    /**
     * @brief
     * Checks if the given ErrorType is the same than the one of the last error message on the stack.
     * If there is no error on the stack, returns false.
     * Returns true if the last error message's type is the same than the given type, false otherwise.
     * @return
     * True if the last error  message's type is the same than the given type, false otherwise.
     */
    template<class TErrorType>
    bool is() const {
        return hasErrors() && last().is<TErrorType>();
    }

    /**
     * @brief
     * True if the stack is non empty, false otherwise.
     * It can be used to check if an error happened.
     */
    operator bool () const;

    /**
     * @brief
     * Adds other's ErrorMessages into this instance of Error.
     * @param other
     * @return
     * The instance of error.
     */
     Error& operator << (const Error & other);

    /**
     * @brief
     * Shortcut for this->add(message);
     * @param message
     * The message to add to the stack.
     * @return
     * The instance of error.
     */
    Error &operator <<(const std::string & message);

    /**
     * @brief
     * Shortcut for this->add(errorMessage);
     * @param errorMessage
     * The error message to add to the stack.
     * @return
     * The instance of error.
     */
    Error& operator << (const ErrorMessage & errorMessage);

    /**
     * @brief
     * Prints the stack on the given output stream.
     * @param out
     * The output stream to print the stack on.
     * @return
     * The output stream.
     */
    friend std::ostream& operator <<(std::ostream& out, const Error & error);
};

std::ostream& operator <<(std::ostream& out, const Error & error);

/**
 * @brief
 * A special error. It can be used when there is no need to
 * stack or be aware of errors. ErrorMessages can not be added to its stack.
 */
extern Error _;

}

