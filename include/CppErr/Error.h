#pragma once

#include <CppErr/ErrorMessage.h>

#include <iostream>
#include <vector>
#include <type_traits>
#include <memory>


#define ERR_ADD_T(err, message, type_name) err.add(message, type_name::id(), __FILE__, __LINE__)
#define ERR_ADD(err, message) err.add(message, cpperr::GenericError::id())

namespace cpperr {

class Error;

using Err = const Error &;

/**
 * @brief
 * -> Represents an error. It holds a stack in which new ErrorMessages can
 * be added.
 * -> Please notice that ErrorMessages can be added to the stack even
 * if the Error is a const-variable.
 * -> An alias can be used to use cpperr::Error as const& variable : cpperr::Err.
 * -> Two macros are available to help writting code :
 * ERR_ADD(err, message) : Adds the message to the error, set the error as a GenericError and set filename and line.
 * ERR_ADD_T(err, message, type_name) : Same as ERR_ADD but uses type_name instead of GenericError.
 */
class Error {

public:
    mutable std::vector<ErrorMessage> stack_;

    Error() = default;
    explicit Error(const Error &) = default; // Set as explicit in order to prevent problems when passing Error as a copy parameter.

    /**
     * @brief stack
     * @return
     */
    const std::vector<ErrorMessage>& stack() const;

    /**
     * @brief
     * Does the error holds ErrorMessages on its stack ?
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
    const ErrorMessage &last() const;

    /**
     * @brief
     * Adds an error to the stack.
     * The macro ERROR(err, msg) can be used to
     * add the message 'msg' to the Error 'err'.
     * @param message
     * The message describing the error.
     * @param typeId
     * The id of the type representing the error.
     * @param fileName
     * The filename of the file where the error occured on.
     * @param line
     * The line number where the error occured.
     */
    void add(const std::string & message, int typeId = cpperr::GenericError::id(), const std::string & fileName = "Unknown File", int line = -1) const;

    /**
     * @brief
     * Clears the stack.
     */
    void clear() const;

    /**
     * @brief
     * Checks if the given ErrorType is the same than the last error one.
     * Like the last() function, this function must be called only if there are elements
     * on the stack. This function works only with type inherited from cpperr::ErrorType<id>.
     * Returns true if the last error type is the same than given type, false otherwise.
     * @return
     * True if the last error type is the same than given type, false otherwise.
     */
    template<class TErrorType>
    bool is() const {
        return TErrorType::id() == last().typeId();
    }

    /**
     * @brief
     * True if the stack is non empty, false otherwise.
     * It can be used to check if an error happened.
     */
    operator bool () const;

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

/**
 * @brief
 * A special error. It can be used when there is no need to
 * stack or be aware of errors. ErrorMessages can not be added to its stack.
 */
extern Error _;

}

