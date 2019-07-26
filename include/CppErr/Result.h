#include <CppErr/Error.h>
#include <exception>

#define ERR_RESULT(type, error_message) { cpperr::Error err; ERR_ADD(err, error_message); cpperr::Result<type>::error(err); }

namespace cpperr {

class Exception : public std::exception {
private:
    std::string message_;

public:
    Exception(const Error & error);

    const char * what() const noexcept override;
};

ERR_DECL_NS(cpperr, ResultIsSuccessfulError);

template<typename R>
union ResultStorage {
public:
    R result;
    Error error;
};

template<typename R>
class Result {
public:
    Result(const Result &)  = default;
    Result(Result &&)       = default;

    static Result<R> success(const R & object) {
        Result r;
        r.result_.result = object;
        r.isSuccess_ = true;

        return std::move(r);
    }

    static Result<R> moveSuccess(R & object) {
        Result r;
        r.result_.result = std::move(object);
        r.isSuccess_ = true;

        return std::move(r);
    }

    template<typename ... Args>
    static Result<R> emplaceSuccess(Args && ...args) {
        Result r;
        r.result_.result = R(std::forward(args)...);
        r.isSuccess_ = true;

        return std::move(r);
    }

    static Result<R> error(Result<R> & other) {
        Result<R> r;
        r.result_.error = std::move(other.error());
        r.isSuccess_ = false;

        return std::move(r);
    }

    static Result<R> error(const Error & error = Error()) {
        Result<R> r;
        r.result_.error = error;
        r.isSuccess_ = false;

        return std::move(r);
    }

    static Result<R> error(const ErrorMessage & errorMessage) {
        return error(Error() << errorMessage);
    }

    static Result<R> error(const std::string & message) {
        return error(ErrorMessage(message));
    }

    R& get() const {
        if(isSuccess_)
            return result_.result;

        throw Exception(result_.error);
    }

    Error& error() const {
        if(!isSuccess_)
            return result_.error;

        Error err;
        err.add(ResultIsSuccessfulError::defaultError());
        throw Exception(err);
    }

    R& operator * () override {
        return get();
    }

    R& operator -> () override {
        return get();
    }

    bool hasError() const {
        return !isSuccess_;
    }

    operator bool () const {
        return isSuccess_;
    }

private:
    Result()                = default;

private:
    ResultStorage<R> result_;
    bool isSuccess_;
};

}
