#include "test.h"

int main() {
    Error err;

    test(!err.hasErrors(), "Error not empty after creation.");

    err.add("An error message.");

    test(err.hasErrors(), "Error with error message not added.");

    err.clear();

    test(!err.hasErrors(), "Error should be empty but it's not.");

    err.add("A generic error.");

    size_t found = err.last().message().find("A generic error.");
    test(found != std::string::npos, "Last message is supposed to contain 'A generic error.' but does not.");

    err.add("Another generic error.");

    found = err.last().message().find("Another generic error.");
    test(found != std::string::npos, "Last message is supposed to contain 'Another generic error.' but does not.");

    return 0;
}
