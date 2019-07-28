#include <iostream>

struct Complex {
    double r, i;

    Complex() {
        std::cout << "CTOR\n";
    }
    Complex(const Complex &) {
        std::cout << "COPY CTOR\n";
    }
    Complex(Complex &&) {
        std::cout << "MOVE CTOR\n";
    }
};

struct Test {
    char data[sizeof(Complex)];
    Complex* complex_ = reinterpret_cast<Complex*>(&data);

    Complex& get() {
        return *reinterpret_cast<Complex*>(&data);
    }

    static Test create() {
        Test t;
        Complex c;
        new(t.complex_) Complex(std::move(c));
        t.get().r = 12.;
        t.get().i = 24.;
        return t;
    }
};

int main() {
    Test t = Test::create();

    std::cout << t.get().r << ", " << t.get().i << "\n";
}