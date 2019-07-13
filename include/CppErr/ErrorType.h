#pragma once

#include <CppErr/ErrorMessage.h>

#include <memory>
#include <string>

#define ERR_CLASS_NS(namespace_name, type_name, class_code) \
    class type_name : public cpperr::ErrorType<#namespace_name "" #type_name ""_cpperr_hash> { \
    public: \
        static constexpr char const * const namespaceName =   "" #namespace_name; \
        static constexpr char const * const typeName      =   "" #type_name; \
        static cpperr::ErrorMessage defaultError() { \
            return cpperr::ErrorMessage(id(), namespaceName, typeName); \
        } \
        static cpperr::ErrorMessage defaultError(const std::string & message) { \
            return cpperr::ErrorMessage(id(), namespaceName, typeName, message); \
        } \
    private: \
        class_code }

#define ERR_CLASS(type_name, class_code) \
    ERR_CLASS_NS(, type_name, class_code)

#define ERR_DECL_NS(namespace_name, type_name) \
    ERR_CLASS_NS(namespace_name, type_name,)

#define ERR_DECL(type_name) \
    ERR_DECL_NS(, type_name)

constexpr int operator"" _cpperr_hash(const char * s, size_t)
{
    unsigned int hash = 0;

    while(*s)
        hash = hash * 101 + static_cast<unsigned>(*s++);

    return static_cast<int>(hash);
}

namespace cpperr {

/**
 * @brief
 * -> Template class for types representing errors.
 * -> Each error type has an id_value (hash of the class name) calculated at compile-time
 * and used to identify errors to a given type.
 * -> Four macros are available to help writting code :
 * ERR_CLASS_NS(namespace_name, type_name, class_code) Creates a class type_name BUT NOT in the given namespace namespace_name.
 * The created class inherits from ErrorType and has a unique id. The namespace_name is only used to create the unique id, in addition to
 * the type_name.
 * The class code has to be implemented via the class_code parameter. The class code is set as this : class type_name { class_code }.
 * ERR_CLASS(type_name, class_code) Calls ERR_CLASS_NS with an empty namespace_name. See ERR_CLASS_NS for more informations.
 * ERR_DECL_NS(namespace_name, type_name) Calls ERR_CLASS_NS with an empty class_code. See ERR_CLASS_NS for more informations.
 * ERR_DECL(type_name) Calls ERR_DECL_NS with an empty namespace_name. See ERR_DECL_NS for more informations.
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

/**
 * @brief
 * A generic error for errors without specified type.
 */
ERR_DECL_NS(cpperr, GenericError);

}


