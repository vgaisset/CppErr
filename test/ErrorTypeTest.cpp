#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <CppErr/Error.h>

using namespace cpperr;

ERR_CLASS(ClassName, {
public :
    static std::string aFunction() { return "a string"; }
};)

ERR_CLASS_NS(ns, ClassName, {
public:
    static std::string aFunction() { return "a string"; }
};)

ERR_DECL(DeclName);
ERR_DECL_NS(ns, DeclName);

TEST_CASE("Error type tests", "[CppErr]") {

    SECTION("Class (ERR_CLASS* or ERR_DECL*) with same name but in different namespace must have diffenrent IDs.") {
        REQUIRE(ClassName::id() != ns::ClassName::id());
        REQUIRE(DeclName::id() != ns::DeclName::id());
    }

    SECTION("Class created with ERR_CLASS* must be implementable.") {
        REQUIRE(ClassName::aFunction() == "a string"); // What matters here is more that we can implement the class than the result of the function call itself.
        REQUIRE(ns::ClassName::aFunction() == "a string");
    }

    SECTION("function defaultError must return an error message with ErrorType id and a custom message.") {
        const ErrorMessage msg = DeclName::defaultError("A test message.");

        REQUIRE(msg.message() == "A test message.");
        REQUIRE(msg.typeId() == DeclName::id());

        SECTION("The error message can use the is<type> function to check if it corresponds to an ErrorType.") {
            REQUIRE(msg.is<DeclName>());
            REQUIRE(!msg.is<ns::DeclName>());
        }
    }
}
