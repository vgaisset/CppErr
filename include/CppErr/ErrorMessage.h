#pragma once

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
    static std::string defaultMessage;

    int          typeId_;
    const char * namespaceName_;
    const char * typeName_;
    std::string message_;

public:
    ErrorMessage();
    ErrorMessage(int typeId, const char * namespaceName, const char * typeName);
    ErrorMessage(const std::string & message);
    ErrorMessage(int typeId, const char * namespaceName, const char * typeName, const std::string & message);

    /**
     * @brief
     * Returns the type id representing this error.
     * @return
     * The type id.
     */
    int typeId() const;

    const char * namespaceName() const;

    const char * typeName() const;

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

/**
 * @brief
 * An error message associated to a filename and a line number;
 */
class LocatedErrorMessage {
public:
    static std::string  defaultFileName;
    static int          defaultLineNumber;

private:
    ErrorMessage        errorMessage_;
    std::string         fileName_;
    int                 line_;

public:
    LocatedErrorMessage(const ErrorMessage & errorMessage, const std::string & fileName, int line);

    /**
     * @brief
     * Returns the type id representing this error.
     * @return
     * The type id.
     */
    int typeId() const;

    const char * namespaceName() const;

    const char * typeName() const;

    /**
     * @brief
     * Returns the message describing the error.
     * @return
     * The message describing the error.
     */
    const std::string & message() const;


    /**
     * @brief
     * Returns the filename of where the error was created.
     * If there is no filename set, it returns LocatedErrorMessage::defaultFileName.
     * @return
     * The filename of where the error was created.
     */
    const std::string & fileName() const;

    /**
     * @brief
     * Returns the line number of where the error was created.
     * If there is no line nimber set, it returns LocatedErrorMessage::defaultLineNumber.
     * @return
     * The line number of where the error was created.
     */
    int line() const;

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
        return errorMessage_.is<TErrorType>();
    }

    friend std::ostream& operator <<(std::ostream& out, const LocatedErrorMessage & errorMessage);
};

std::ostream& operator <<(std::ostream& out, const LocatedErrorMessage & errorMessage);

}

