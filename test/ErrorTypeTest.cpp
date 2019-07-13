#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <CppErr/Error.h>

using namespace cpperr;

ERR_CLASS(ClassName,
public :
    static std::string aFunction() { return "a string"; }
    static int sum(int a, int b) { return a + b; }
);

namespace ns {

    ERR_CLASS_NS(ns, ClassName,
    public:
        static std::string aFunction() { return "a string"; }
    );

    ERR_DECL_NS(ns, DeclName);

}

ERR_DECL(DeclName);

TEST_CASE("Error type tests", "[CppErr]") {

    SECTION("Class (ERR_CLASS* or ERR_DECL*) with same name but in different namespace must have diffenrent IDs.") {
        REQUIRE(ClassName::id() != ns::ClassName::id());
        REQUIRE(DeclName::id() != ns::DeclName::id());
    }

    SECTION("Class created with ERR_CLASS* must be implementable.") {
        REQUIRE(ClassName::aFunction() == "a string"); // What matters here is more that we can implement the class than the result of the function call itself.
        REQUIRE(ns::ClassName::aFunction() == "a string");
    }

    SECTION("Implemented functions can have multiple parameters.") {
        REQUIRE(ClassName::sum(1, 2) == 3);
    }

    SECTION("function defaultError must return an error message with ErrorType id and a custom message.") {
        ErrorMessage msg = ns::DeclName::defaultError("A test message.");

        REQUIRE(msg.message() == "A test message.");
        REQUIRE(msg.typeId() == ns::DeclName::id());
        REQUIRE(msg.typeName() == "DeclName");
        REQUIRE(msg.namespaceName() == "ns");

        msg = DeclName::defaultError();

        REQUIRE(msg.namespaceName() == "");

        SECTION("The error message can use the is<type> function to check if it corresponds to an ErrorType.") {
            REQUIRE(msg.is<DeclName>());
            REQUIRE(!msg.is<ns::DeclName>());
        }
    }
}
