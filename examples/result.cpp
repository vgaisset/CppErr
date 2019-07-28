#include <CppErr/Result.h>

using namespace cpperr;

struct Complex {
    double r, i;

    Complex() {
        std::cout << "CTOR\n";
    }
    Complex(const Complex & c) : r{c.r}, i{c.i} {
        std::cout << "COPY CTOR\n";
    }
    Complex(Complex && c) : r{std::move(c.r)}, i{std::move(c.i)} {
        std::cout << "MOVE CTOR\n";
    }
};

Result<Complex> getErrorResult() {
    return ERR_RESULT(Complex, "Invalid number.");
}

int main() {
    auto r = getErrorResult();

    std::cout << r.error() << std::endl;
    if(r.error().hasErrors())
        std::cout << "Has errors.";

    return 0;
}