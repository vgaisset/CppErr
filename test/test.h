#include <CppErr/Error.h>

using namespace cpperr;

void test(bool result, const std::string & msg) {
    if(!result)
    {
        std::cerr << msg;
        exit(1);
    }
}
