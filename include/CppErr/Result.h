#include <CppErr/Error.h>
#include <exception>

#include <type_traits>

#define ERR_RESULT(type, error_message) cpperr::Result<type>::emplaceError(error_message, __FILE__, __LINE__)

namespace cpperr {

/**
 * @brief
 * -> An exception holding a message.
 * If the exception is constructed using an Error, the stack trace
 * is put in the message.
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
            new (dataAsResult()) R(*other.dataAsConstResult());
        else
            new (dataAsError()) E(*other.dataAsConstError());
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
     * Calls the R copy constrcutor.
     * @param objectToCopy
     */
    Expected(const R & objectToCopy) :
        isSuccess_{true}
    {
        new (dataAsResult()) R(objectToCopy);
    }

    /**
     * @brief
     * Calls the R move constructor.
     * @param objectToMove
     */
    Expected(R && objectToMove) :
        isSuccess_{true}
    {
        new (dataAsResult()) R(objectToMove);
    }

    /**
     * @brief
     * Creates an object of type R using the given args.
     * @param args
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
     * The Expected object holding the error.
     */
    template<typename OtherR>
    static Expected<R, E> errorFromResult(Expected<OtherR, E> & other) {
        Expected<R, E> r;
        new (r.dataAsError()) E(std::move(other.error()));
        r.isSuccess_ = false;

        return r;
    }

    /**
     * @brief
     * Creates an Expected object, copies the given error and returns it.
     * @param error
     * @return
     * The Expected object holding the error.
     */
    static Expected<R, E> error(const E & error) {
        Expected<R, E> r;
        new (r.dataAsError()) E(error);
        r.isSuccess_ = false;

        return r;
    }

    /**
     * @brief
     * Creates an Expected object, moves the given error and returns it.
     * @param error
     * @return
     * The Expected object holding the error.
     */
    static Expected<R, E> error(E && error) {
        Expected<R, E> r;
        new (r.dataAsError()) E(error);
        r.isSuccess_ = false;

        return r;
    }

    /**
     * @brief
     * Creates an Expected object, emplaces an error using the given args
     * and returns it.
     * @param args
     * @return
     * The Expected object holding the error.
     */
    template<typename ...Args>
    static Expected<R, E> emplaceError(Args && ...args) {
        Expected<R, E> r;
        new (r.dataAsError()) E(std::forward<Args>(args)...);
        r.isSuccess_ = false;

        return r;
    }

    /**
     * @brief
     * Returns a reference to the held R object.
     * If there is an error and no R object, a cpperr::Exception
     * is thrown instead.
     * @return
     * A reference to the held R object.
     */
    R& result() {
        throwIfError();

        return *dataAsResult();
    }

    /**
     * @brief
     * Returns a reference to the held E object.
     * If there is no error but a R object, a cpperr::Exception is thrown
     * @return
     */
    E& error() {
        if(hasError())
            return *dataAsError();

        throw Exception();
    }

    /**
     * @brief
     * Throws an Exception if the object is holding an error.
     */
    void throwIfError() const {
        if(!isSuccess_)
            throwError();
    }

    /**
     * @brief
     * Returns a reference to the R object.
     * See result() for more informations.
     * @return
     * A reference to the R object.
     */
    R& operator * () {
        return result();
    }

    /**
     * @brief
     * Returns a pointer to the R object.
     * See result() for more informations.
     * @return
     * A pointer to the R object.
     */
    R* operator -> () {
        return &result();
    }

    /**
     * @brief
     * Does the object hold an error ?
     * @return
     * True if the object holds an object of type E, false otherwise.
     */
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

    inline const R * dataAsConstResult() const {
        return reinterpret_cast<const R *>(&data_);
    }

    inline const E * dataAsConstError() const {
        return reinterpret_cast<const E *>(&data_);
    }

    /**
     * @brief
     * Throws the error held by the object.
     * If the object is not holding an error, the behavior is undefined.
     * If cpperr::Error is a base of E, the thrown exception is constrcuted
     * using the error held. Otherwise, an Exception without informations is thrown.
     */
    void throwError() const {
        if constexpr(std::is_base_of<Error, E>::value)
            throw Exception(*dataAsConstError());
        else
            throw Exception();
    }

private:
    /**
     * @brief
     * Holds R or E content.
     */
    char data_[ max_size(sizeof(R), sizeof(E)) ];

    /**
     * @brief
     * Is the Expected object holding a R object ?
     */
    bool isSuccess_;
};

/**
 * @brief
 * Represents a cpperr::Expected object with cpperr::Error.
 */
template<typename R>
using Result = Expected<R, Error>;

}
