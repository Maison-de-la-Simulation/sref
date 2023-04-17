#pragma once

#include <functional>
#include <memory>
#include <stdexcept>

namespace sref {

template <class T, class Deleter>
class unique_ref;

template <class T>
class shared_ref
{
private:
    template <class U>
    friend class shared_ref;

    std::shared_ptr<T> m_ptr;

public:
    using type = T;

    template <class U>
    explicit shared_ref(std::shared_ptr<U> ptr) : m_ptr(std::move(ptr))
    {
        if (!m_ptr) {
            throw std::invalid_argument("nullptr");
        }
    }

    template <class U>
    shared_ref(shared_ref<U> ref) noexcept : m_ptr(std::move(ref.m_ptr))
    {
    }

    template <class U, class ODeleter>
    shared_ref(unique_ref<U, ODeleter> ref) noexcept
        : m_ptr(std::unique_ptr<U, ODeleter>(std::move(ref)))
    {
    }

    shared_ref(shared_ref const& rhs) = default;

    shared_ref(shared_ref&& rhs) noexcept = default;

    ~shared_ref() noexcept = default;

    shared_ref& operator=(shared_ref const& rhs) = default;

    shared_ref& operator=(shared_ref&& rhs) noexcept = default;

    T& operator*() const noexcept
    {
        return *m_ptr;
    }

    T* operator->() const noexcept
    {
        return m_ptr.get();
    }

    template <class... Args>
    constexpr std::invoke_result_t<T&, Args...> operator()(Args&&... args) const
    {
        return std::invoke(*m_ptr, std::forward<Args>(args)...);
    }
};

template <class T, class Deleter>
shared_ref(unique_ref<T, Deleter> ref) -> shared_ref<T>;

template <class T>
shared_ref<T> as_ref(std::shared_ptr<T> ptr) noexcept
{
    return shared_ref<T>(std::move(ptr));
}

template <class T>
shared_ref<std::add_const_t<T>> as_cref(std::shared_ptr<T> ptr) noexcept
{
    return shared_ref<T>(std::move(ptr));
}

template <class T, class... Args>
shared_ref<T> make_shared(Args&&... args)
{
    return as_ref(std::make_shared<T>(std::forward<Args>(args)...));
}

} // namespace sref
