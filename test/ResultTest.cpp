#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <CppErr/Result.h>

using namespace cpperr;

struct Complex {
    double r, i;

    Complex(double c_r, double c_i) : r{c_r}, i{c_i} {}
};

Result<Complex> getErrorResult() {
    return ERR_RESULT(Complex, "Invalid number.");
}
#include <iostream>
TEST_CASE("Result tests", "Tests the Result features.") {

    SECTION("A bad result must have error, no result object, must throw an error when accessing object and return an error.") {
        Result<Complex> r = ERR_RESULT(Complex, "Invalid number.");//getErrorResult();

        REQUIRE(r.hasError()); // Result has an error.
        REQUIRE(!r); // Result not successful.

        REQUIRE_THROWS_AS(r.result(), Exception); // Getting the result throws a cpperr::Exception.
        REQUIRE_THROWS_AS(*r, Exception); // Same as r.result().
        REQUIRE_THROWS_AS(r->i, Exception); // Same as r.result().

        REQUIRE(r.error().hasErrors()); // Getting error must not throw and error (in this case) must not be empty.
        REQUIRE(r.error().last().is<GenericError>());
        REQUIRE(r.error().last().message() == "Invalid number.");

        auto r2 = Result<Complex>::errorFromResult(r);
    }

    SECTION("A good result must not have an error, must have the successfully created object and requesting error must throw.") {
        Result<Complex> r = Result<Complex>::emplaceSuccess(42., 12.);

        REQUIRE(!r.hasError());
        REQUIRE(r);

        REQUIRE_THROWS_AS(r.error(), Exception);

        REQUIRE(&(*r) == &r.result());

        REQUIRE(r->r == 42.);
        REQUIRE(r->i == 12.);
    }
}
