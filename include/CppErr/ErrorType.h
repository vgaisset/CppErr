#pragma once

#include <CppErr/ErrorMessage.h>

#include <string>

#define ERR_CLASS(type_name, class_code) \
    class type_name : public cpperr::ErrorType<#type_name ""_cpperr_hash> class_code

#define ERR_CLASS_NS(namespace_name, type_name, class_code) \
    namespace namespace_name { \
        class type_name : public cpperr::ErrorType<#namespace_name "" #type_name ""_cpperr_hash> class_code \
    }

#define ERR_DECL(type_name) \
    ERR_CLASS(type_name, {};)

#define ERR_DECL_NS(namespace_name, type_name) \
    ERR_CLASS_NS(namespace_name, type_name, {};)

namespace cpperr {

/**
 * @brief
 * -> Template class for types representing errors.
 * -> Each error type has an id_value (hash of the class name) calculated at compile-time
 * and used to identify errors to a given type.
 * -> Three macros are available to help writting code :
 * ERR_CLASS(type_name, class_code) Creates a class named type_name which inherits from ErrorType with a unique id.
 * The id is generated using class name hash.
 * The class code has to be implemented via the class_code parameter.
 * ERR_CLASS_NS(namespace_name, type_name, class_code) Creates a class in the given namespace, which inherits from ErrorType with a unique id.
 * The id is generated using both namespace name and class name hash.
 * The class code has to be implemented via the class_code parameter.
 * ERR_DECL(type_name) Calls ERR_CLASS with an empty class implementation : {};.
 * ERR_DECL_NS(namespace_name, type_name) Calls ERR_CLASS_NS with an empty class implementation : {};.
 */
template<int id_value>
class ErrorType
{
public:
    ErrorType() = default;

    /**
     * @brief
     * Returns the id representing the ErrorType.
     * @return
     * The id.
     */
    constexpr static int id() {
        return id_value;
    }

    /**
     * @brief
     * Creates an ErrorMessage associated to the current ErrorType with no message
     * and returns it.
     * @return
     * A new ErrorMessage with no message and associated to the current ErrorType.
     */
    static ErrorMessage defaultError() {
        return ErrorMessage(id_value);
    }

    /**
     * @brief
     * Creates an ErrorMessage associated to the current ErrorType with the given message
     * and returns it.
     * @param message
     * The message to use in the created ErrorMessage.
     * @return
     * A new ErrorMessage with the given message and associated to the current ErrorType.
     */
    static ErrorMessage defaultError(const std::string & message) {
        return ErrorMessage(id_value, message);
    }
};

}

constexpr int operator"" _cpperr_hash(const char * s, size_t)
{
    unsigned int hash = 0;

    while(*s)
        hash = hash * 101 + static_cast<unsigned>(*s++);

    return static_cast<int>(hash);
}

/**
 * @brief
 * A generic error for errors without specified type.
 */
ERR_DECL_NS(cpperr, GenericError);

