#pragma once

#include <string>

#define ERR_CLASS(type_name) class type_name : public cpperr::ErrorType<#type_name ""_cpperr_hash>
#define ERR_DECL(type_name) ERR_CLASS(type_name) {};
#define ERR_DECL_NS(namespace_name, type_name) namespace namespace_name { ERR_DECL(type_name); }

namespace cpperr {

/**
 * @brief
 * -> Template class for types representing errors.
 * -> Each error type has an id_value (hash of the class name) calculated at compile-time
 * and used to identify errors to a given type.
 * -> Three macros are available to help writting code :
 * ERR_CLASS(type_name) Creates a class named type_name which inherits from ErrorType with a unique id.
 * The class is not implemented.
 * ERR_DECL(type_name) Uses ERR_CLASS and closes the class. Here the class only represents an error type.
 * ERR_DECL_NS(namespace_name, type_name) Wraps the class created by ERR_DECL in the namespace namespace_name.
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
};

}

constexpr int operator"" _cpperr_hash(const char * s, std::size_t)
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

