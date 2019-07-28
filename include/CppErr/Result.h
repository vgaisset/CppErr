#include <CppErr/Error.h>
#include <exception>

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

template<typename R>
class Result {
public:
    /**
     * @brief
     * Copy constructor.
     * Explicitly call copy constructor for object contained in the result.
     * @param other
     * The Result to copy.
     */
    Result(const Result & other) :
        isSuccess_{other.isSuccess_}
    {
        if(isSuccess_)
            new (dataAsResult()) R(*reinterpret_cast<const R *>(other.data_));
        else
            new (dataAsError()) Error(*reinterpret_cast<const Error *>(other.data_));
    }

    /**
     * @brief
     * Move constructor.
     * Explicitly call move constructor for object contained in the result.
     * @param other
     * The Result to move.
     */
    Result(Result && other) :
        isSuccess_{std::move(other.isSuccess_)}
    {
        if(isSuccess_)
            new (dataAsResult()) R(std::move(*reinterpret_cast<R *>(other.data_)));
        else
            new (dataAsError()) Error(std::move(*reinterpret_cast<Error *>(other.data_)));
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
    static Result<R> copySuccess(const R & objectToCopy) {
        Result r;
        new (r.dataAsResult()) R(objectToCopy);
        r.isSuccess_ = true;

        return std::move(r);
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
    static Result<R> moveSuccess(R && objectToMove) {
        Result r;
        new (r.dataAsResult()) R(objectToMove);
        r.isSuccess_ = true;

        return std::move(r);
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
    static Result<R> emplaceSuccess(Args && ...args) {
        Result r;
        new (r.dataAsResult()) R(std::forward<Args>(args)...);
        r.isSuccess_ = true;

        return std::move(r);
    }

    /**
     * @brief
     * Creates a Result<R> with an error and moves
     * @param other
     * @return
     */
    static Result<R> errorFromResult(Result<R> & other) {
        Result<R> r;
        new (r.dataAsError()) Error(std::move(other.error()));
        r.isSuccess_ = false;

        return std::move(r);
    }

    static Result<R> copyError(const Error & error) {
        Result<R> r;
        new (r.dataAsError()) Error(error);
        r.isSuccess_ = false;

        return std::move(r);
    }

    static Result<R> moveError(Error && error) {
        Result<R> r;
        new (r.dataAsError()) Error(error);
        r.isSuccess_ = false;

        return std::move(r);
    }

    template<typename ...Args>
    static Result<R> emplaceError(Args && ...args) {
        Result<R> r;
        new (r.dataAsError()) Error(std::forward<Args>(args)...);
        r.isSuccess_ = false;

        return std::move(r);
    }

    R& result() {
        if(isSuccess_)
            return *dataAsResult();

        throw Exception(*dataAsError());
    }

    Error& error() {
        if(hasError())
            return *dataAsError();

        Error err;
        err.add(ResultIsSuccessfulError::defaultError());
        throw Exception(err);
    }

    Result<R> throwIfError() {
        if(hasError())
            throw Exception(*dataAsError());
        return *this;
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

    ~Result() {
        if(isSuccess_)
            dataAsResult()->~R();
        else
            dataAsError()->~Error();
    }
private:
    Result()                = default;

    inline R* dataAsResult() {
        return reinterpret_cast<R*>(&data_);
    }

    inline Error* dataAsError() {
        return reinterpret_cast<Error*>(&data_);
    }

private:
    // data holds result_ or error_ content.
    char data_[max_size(sizeof(R), sizeof(Error))];

    bool isSuccess_;
};

}
