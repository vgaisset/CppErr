# CppErr

CppErr is a minimalistic library for error handling in C++.
You can create errors, stack them, give a type to an error and show stacktrace. 

## Installation

You can use CMake to compile the library :

```bash
git clone https://github.com/vgaisset/CppErr.git
cd CppErr
mkdir build && cd build
cmake .. # Options can be added here.
make
sudo make install
```

Options can be provided to cmake :

 * CPPERR_BUILD_TESTS can be used to build (or not) unit tests (default = OFF). Please note that catch2 is needed to build unit tests.
 * CPPERR_BUILD_STATIC can be used to choose if the library must be compiled as STATIC instead of SHARED (default = OFF).

Then, if you're using cmake for you project, you can add the library using :

```cmake
find_package(CppErr REQUIRED)
# ...
add_executable(${PROJECT_NAME} ${src})
# ...
target_link_libraries(${PROJECT_NAME} PUBLIC CppErr)
```

or with the -lCppErr flag if you're compiling manually.

## Usage

The only include you need is :
```c++
#include <CppErr/Error.h>
```
Then, you can create error and add error messages to it :

```c++
cpperr::Error err;

// No informations about file / line.
err.add("An error message."); 
err << "An error message";

// The same error message, but with informations about file / line.
err.add("An error message.", __FILE__, __LINE__); 
ERR_ADD(err, "An error message."); // The macro calls the add function with __FILE__ and __LINE__ parameters.

// Prints the stack trace to the console.
std::cout << err;
```

This error contains the following stack :

```
=============== STACK TRACE (4)

=(1)= first -> GenericError from namespace cpperr (id = -1502749837)
=== At Unknown file, on line -1
An error message.

=(2)= Then  -> GenericError from namespace cpperr (id = -1502749837)
=== At Unknown file, on line -1
An error message

=(3)= Then  -> GenericError from namespace cpperr (id = -1502749837)
=== At test.cpp, on line 12
An error message.

=(4)= Then  -> GenericError from namespace cpperr (id = -1502749837)
=== At test.cpp, on line 13
An error message.

=============== 4 errors shown
```

You may have noticed that error messages were of type GenericError.
In fact, error messages are represented by an error type. By default, its cpperr::GenericError, but you can create your own error types in order to represent a specific error message.

Here's how to create your own types using the **ERR_DECL** macro :

```c++
ERR_DECL(MyCustomError); // Creates an ErrorType named MyCustomError.

int main() {
    cpperr::Error err;

    // Adds an error message with the MyCustomError type, with a message.
    ERR_ADD(err, MyCustomError::defaultError("A custom error with a message"));

    // The same as above, but without message.
    ERR_ADD(err, MyCustomError::defaultError());

    // You can also check error messages' type.
    if(err.is<MyCustomError>())
        std::cout << err;
   
    return 0;
}
```

And here is the output :

```
=============== STACK TRACE (2)

=(1)= first -> MyCustomError (id = 2021577267)
=== At test.cpp, on line 10
A custom error with a message

=(2)= Then  -> MyCustomError (id = 2021577267)
=== At test.cpp, on line 13

=============== 2 errors shown
```

Also, you can implements your own functions on your error types using the **ERR_CLASS** macro.
Let's take the example above back :

```c++
ERR_CLASS(MyCustomError,
public:
    static cpperr::ErrorMessage newError(const std::string & msg) {
        return defaultError("My implementaion : " + msg);
    }
); // Creates an ErrorType named MyCustomError.

int main() {
    cpperr::Error err;

    // Adds an error message with the MyCustomError type, with a message.
    ERR_ADD(err, MyCustomError::defaultError("A custom error with a message"));

    // The same as above, but without message.
    ERR_ADD(err, MyCustomError::newError("a message."));

    // You can then check error messages' type.
    if(err.is<MyCustomError>())
        std::cout << err;
   
    return 0;
}
```

And then the output :

```
=============== STACK TRACE (2)

=(1)= first -> MyCustomError (id = 2021577267)
=== At test.cpp, on line 15
A custom error with a message

=(2)= Then  -> MyCustomError (id = 2021577267)
=== At test.cpp, on line 18
My implementaion : a message.

=============== 2 errors shown
```

Please note that you can specify namespaces where your error types are created using the **ERR_CLASS_NS** and **ERR_DECL_NS** macros. Namespaces are important to avoid clash when checking error type for two error types with the same name. When there is a namespace specified, it's taken in account when creating the error type's id.

For more advanced examples check [here](https://github.com/vgaisset/CppErr/tree/master/examples).


## Contributing
All contributions are welcome. Do not hesitate to perform pull request or to open an issue to discuss about a feature / modification you would like to see.

## License
[MIT](https://choosealicense.com/licenses/mit/)
