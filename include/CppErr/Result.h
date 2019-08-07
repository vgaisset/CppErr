#include <CppErr/Error.h>
#include <exception>

#include <type_traits>

#define ERR_RESULT(type, error_message) cpperr::Result<type>::emplaceError(error_message, __FILE__, __LINE__)

namespace cpperr {

/**
 * @brief
 * -> An exception constructed using a cpperr::Error.
 * The stack trace contained in the error is saved as a string
 * and can be accessed through the what() function.
 * -> This exception is thrown when accessing an invalid field
 * in a cpperr::Result object.
 */
class Exception : public std::exception {
private:
    std::string message_;

public:
    Exception();
    Exception(const Error & error);

    const char * what() const noexcept override;
};

ERR_DECL_NS(cpperr, ResultIsSuccessfulError);

/**
 * @brief
 * Constexpr function which returns the maximum
 * between to size_t. Can be used at compile-time.
 * @param left
 * The first size_t to compare.
 * @param right
 * The second size_t to compare.
 * @return
 * The max between left and right.
 */
constexpr size_t max_size(size_t left, size_t right) {
    return left > right ? left : right;
}

template<typename R, typename E>
class Expected {
public:
    /**
     * @brief
     * Copy constructor.
     * Explicitly call copy constructor for object contained in the result.
     * @param other
     * The Result to copy.
     */
    Expected(const Expected & other) :
        isSuccess_{other.isSuccess_}
    {
        if(isSuccess_)
            new (dataAsResult()) R(*other.dataAsResult());
        else
            new (dataAsError()) E(*other.dataAsError());
    }

    /**
     * @brief
     * Move constructor.
     * Explicitly call move constructor for object contained in the result.
     * @param other
     * The Result to move.
     */
    Expected(Expected && other) :
        isSuccess_{std::move(other.isSuccess_)}
    {
        if(isSuccess_)
            new (dataAsResult()) R(std::move(*other.dataAsResult()));
        else
            new (dataAsError()) E(std::move(*other.dataAsError()));
    }

    /**
     * @brief
     * Creates a Result<R> containing an object of type R and calls
     * the copy constructor with the objectToCopy as parameter.
     * @param objectToCopy
     * The R object given to the object constructor.
     * @return
     * A new Result<R>.
     */
    Expected(const R & objectToCopy) :
        isSuccess_{true}
    {
        new (dataAsResult()) R(objectToCopy);
    }

    /**
     * @brief
     * Creates a Result<R> containing an object of type R and calls
     * the move constructor with the objectToMove as parameter.
     * @param objectToMove
     * The R object given to the object constructor.
     * @return
     * A new Result<R>.
     */
    Expected(R && objectToMove) :
        isSuccess_{true}
    {
        new (dataAsResult()) R(objectToMove);
    }

    /**
     * @brief
     * Creates a Result<R> and emplace an object R.
     * @param args
     * The R object constrcutor arguments.
     * @return
     * A new Result<R>.
     */
    template<typename ... Args>
    Expected(Args && ...args) :
        isSuccess_{true}
    {
        new (dataAsResult()) R(std::forward<Args>(args)...);
    }

    /**
     * @brief
     * Creates a Result<R> with an error and moves
     * @param other
     * @return
     */
    template<typename OtherR>
    static Expected<R, E> errorFromResult(Expected<OtherR, E> & other) {
        Expected<R, E> r;
        new (r.dataAsError()) E(std::move(other.error()));
        r.isSuccess_ = false;

        return r;
    }

    static Expected<R, E> error(const E & error) {
        Expected<R, E> r;
        new (r.dataAsError()) E(error);
        r.isSuccess_ = false;

        return r;
    }

    static Expected<R, E> error(E && error) {
        Expected<R, E> r;
        new (r.dataAsError()) E(error);
        r.isSuccess_ = false;

        return r;
    }

    template<typename ...Args>
    static Expected<R, E> emplaceError(Args && ...args) {
        Expected<R, E> r;
        new (r.dataAsError()) E(std::forward<Args>(args)...);
        r.isSuccess_ = false;

        return r;
    }

    R& result() {
        if(isSuccess_)
            return *dataAsResult();

        throw Exception(*dataAsError());
    }

    E& error() {
        if(hasError())
            return *dataAsError();

        throw Exception();
    }

    void throwIfError() {
        if(hasError()) {
            if constexpr(std::is_base_of<Error, E>::value) {
                throw Exception();
            } else {
                throw Exception(*dataAsError());
            }
        }
    }

    R& operator * () {
        return result();
    }

    R* operator -> () {
        return &result();
    }

    bool hasError() const {
        return !isSuccess_;
    }

    operator bool () const {
        return isSuccess_;
    }

    ~Expected() {
        if(isSuccess_)
            dataAsResult()->~R();
        else
            dataAsError()->~E();
    }
private:
    Expected()                = default;

    inline R* dataAsResult() {
        return reinterpret_cast<R*>(&data_);
    }

    inline E* dataAsError() {
        return reinterpret_cast<E*>(&data_);
    }

private:
    // data holds R or E content.
    char data_[max_size(sizeof(R), sizeof(E))];

    bool isSuccess_;
};

template<typename R>
using Result = Expected<R, Error>;

}
